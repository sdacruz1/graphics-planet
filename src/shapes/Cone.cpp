#include "Cone.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <ostream>

void Cone::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

// Create a triangle based on 3 given points and 3 given vectors.
void Cone::makeTriangle(std::vector<glm::vec3> points, std::vector<glm::vec3> normals) {
    insertVec3(m_vertexData, points[0]);
    insertVec3(m_vertexData, normals[0]);
    insertVec3(m_vertexData, points[1]);
    insertVec3(m_vertexData, normals[1]);
    insertVec3(m_vertexData, points[2]);
    insertVec3(m_vertexData, normals[2]);
}

// Create a tile based on 4 given points.
void Cone::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {

    glm::vec3 TLnormal = glm::normalize(glm::vec3(2.0 * topLeft[0], -0.5 * topLeft[1] + 0.25, 2.0 * topLeft[2]));
    glm::vec3 TRnormal = glm::normalize(glm::vec3(2.0 * topRight[0], -0.5 * topRight[1] + 0.25, 2.0 * topRight[2]));
    glm::vec3 BLnormal = glm::normalize(glm::vec3(2.0 * bottomLeft[0], -0.5 * bottomLeft[1] + 0.25, 2.0 * bottomLeft[2]));
    glm::vec3 BRnormal = glm::normalize(glm::vec3(2.0 * bottomRight[0], -0.5 * bottomRight[1] + 0.25, 2.0 * bottomRight[2]));

    // triangle 1
    std::vector<glm::vec3> points = {topLeft, bottomLeft, bottomRight};
    std::vector<glm::vec3> normals = {TLnormal, BLnormal, BRnormal};
    makeTriangle(points, normals);

    // triangle 2
    std::vector<glm::vec3> points2 = {topLeft, bottomRight, topRight};
    std::vector<glm::vec3> normals2 = {TLnormal, BRnormal, TRnormal};
    makeTriangle(points2, normals2);
}

// Create a slice of the cone base
void Cone::createBaseSlice(float bottomLeftPointX, float bottomLeftPointY, float bottomRightPointX, float bottomRightPointY) {

    // Set the cap's height and normal direction
    float z = -0.5;
    glm::vec3 normal = glm::vec3(0.0, -1.0, 0.0);

    // Determine the x and y increments for tesselation
    glm::vec2 incrementLeft = glm::vec2(bottomLeftPointX * (1.0 / (m_param1)), bottomLeftPointY * (1.0 / (m_param1)));
    glm::vec2 incrementRight = glm::vec2(bottomRightPointX * (1.0 / (m_param1)), bottomRightPointY * (1.0 / (m_param1)));

    // The left and right points of the first slice of the base triangle are one increment out from the origin
    glm::vec3 left = glm::vec3(incrementLeft[0], z, incrementLeft[1]);
    glm::vec3 right = glm::vec3(incrementRight[0], z, incrementRight[1]);

    // make the first triangle of the base
    std::vector<glm::vec3> points = {right, glm::vec3(0.0, z, 0.0), left};
    std::vector<glm::vec3> normals = {normal, normal, normal};
    makeTriangle(points, normals);

    // tesselation
    glm::vec3 nextLeft;
    glm::vec3 nextRight;
    for (int h = 2; h <= m_param1; h++) {
        //nextLeft = left + 1 step away from the origin, same for nextRight
        nextLeft = glm::vec3(h * incrementLeft[0], z, h * incrementLeft[1]);
        nextRight = glm::vec3(h * incrementRight[0], z, h * incrementRight[1]);

        // set up vectors for makeTriangle
        std::vector<glm::vec3> points1;
        std::vector<glm::vec3> normals1;
        std::vector<glm::vec3> points2;
        std::vector<glm::vec3> normals2;

        // to continue building the base, we make a new tile out of two triangles

        // triangle one
        // left and right connect to nextLeft
        points1 = {left, nextLeft, right};
        normals1 = {normal, normal, normal};
        makeTriangle(points1, normals1);

        // triangle two
        // nextLeft and right connect to nextRight
        points2 = {right, nextLeft, nextRight};
        normals2 = {normal, normal, normal};
        makeTriangle(points2, normals2);

        //set left to nextLeft and right to nextRight
        left = nextLeft;
        right = nextRight;
    }
}

// Create a unit cone
void Cone::setVertexData() {
    float angleOfSlice, bottomLeftPointX, bottomLeftPointY, bottomRightPointX, bottomRightPointY;
    glm::vec2 stepUp, stepUp2;

    // The number of vertical slices that make up the cone is equal to param2
    // A cone is symmetrical about 2pi degrees, so we can split it into param2 slices each containing
    // 2pi / param2 degrees of the cone.
    float pi = 3.14159265;
    angleOfSlice = ((2 * pi) / m_param2);

    // For each slice, create a slice of base and a slice of the wall
    for (int slice = 0; slice < m_param2; slice ++) {

        //************* GET THE COORDINATES OF THE BOTTOM *************//

        // These are the X and Y coordinates of the left point of the slice
        bottomLeftPointX = 0.5 * cos(slice * angleOfSlice);
        bottomLeftPointY = 0.5 * sin(slice * angleOfSlice);

        // These are the X and Y coordinates of the right point of the slice
        bottomRightPointX = 0.5 * cos((slice + 1) * angleOfSlice);
        bottomRightPointY = 0.5 * sin((slice + 1) * angleOfSlice);

        //************* MAKE THE BASE OF THE SLICE *************//

        createBaseSlice(bottomLeftPointX, bottomLeftPointY, bottomRightPointX, bottomRightPointY);

        //************* MAKE THE WALL OF THE SLICE *************//

        // There are 1 / param1 horizontal slices making up the side of the wall.
        // Therefore each slice will have a height of: side_height / param1
        float slice_height = (1.0 / m_param1);

        // Get the top triangle
        glm::vec3 positionOfConeTop = glm::vec3(0.0, 0.5, 0.0);

        // Create a vector from the top of the cone to the bottom left and bottom right point of the horizontal slice
        glm::vec3 vectorTopToBottomL = glm::vec3(bottomLeftPointX,  -0.5,  bottomLeftPointY) - positionOfConeTop;
        glm::vec3 vectorTopToBottomR = glm::vec3(bottomRightPointX, -0.5, bottomRightPointY) - positionOfConeTop;

        // That vector times the slice height (aka. 1 / the number of tesselations in this direction) gives an increment offset
        glm::vec3 offsetLeft = slice_height * vectorTopToBottomL;
        glm::vec3 offsetRight = slice_height * vectorTopToBottomR;

        // Go one increment down from the top in the left and right direction to get the bottom vertices of the topmost triangle
        glm::vec3 positionOfBottomLeft  = offsetLeft + positionOfConeTop;
        glm::vec3 positionOfBottomRight = offsetRight + positionOfConeTop;

        // the bottom of the current tile will be the top of the next tile
        glm::vec3 positionOfTopLeft = positionOfBottomLeft;
        glm::vec3 positionOfTopRight = positionOfBottomRight;

        // Make the top portion of the cone, starting from the origin and going down by one increment
        std::vector<glm::vec3> points3 = {positionOfBottomLeft, positionOfConeTop, positionOfBottomRight};

        glm::vec3 topTLnormal = glm::normalize(glm::vec3(2.0 * positionOfTopLeft[0], -0.5 * positionOfTopLeft[1] + 0.25, 2.0 * positionOfTopLeft[2]));
        glm::vec3 topTRnormal = glm::normalize(glm::vec3(2.0 * positionOfTopRight[0], -0.5 * positionOfTopRight[1] + 0.25, 2.0 * positionOfTopRight[2]));
        glm::vec3 topNormal = (topTLnormal + topTRnormal) / 2.0f;
        std::vector<glm::vec3> normals3 = {topTLnormal, topNormal, topTRnormal};

        makeTriangle(points3, normals3);

        // For each horizontal slice of the cone below the first, make a new tile.
        // The top of the tile is the bottom of the last tile
        // The bottom of the tile is one increment further down than the top of the tile
        for (int horSlice = 2; horSlice <= m_param1; horSlice++) {
            positionOfBottomLeft  = positionOfTopLeft + offsetLeft;
            positionOfBottomRight = positionOfTopRight + offsetRight;

            makeTile(positionOfTopRight, positionOfTopLeft, positionOfBottomRight, positionOfBottomLeft);

            positionOfTopLeft  = positionOfBottomLeft;
            positionOfTopRight = positionOfBottomRight;
        }
    }
}


// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cone::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
