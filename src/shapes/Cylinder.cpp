#include "Cylinder.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <ostream>

void Cylinder::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

void Cylinder::makeTriangle(std::vector<glm::vec3> points, std::vector<glm::vec3> normals) {
    insertVec3(m_vertexData, points[0]);
    insertVec3(m_vertexData, normals[0]);
    insertVec3(m_vertexData, points[1]);
    insertVec3(m_vertexData, normals[1]);
    insertVec3(m_vertexData, points[2]);
    insertVec3(m_vertexData, normals[2]);
}

    // Create a tile based on 4 given points.
void Cylinder::makeTile(glm::vec3 topRight,
                    glm::vec3 topLeft,
                    glm::vec3 bottomRight,
                    glm::vec3 bottomLeft) {

    // Implicit normals
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


void Cylinder::setVertexData() {
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

        // Determine the x and y increments for tesselation
        glm::vec2 incrementLeft = glm::vec2(bottomLeftPointX * (1.0 / (m_param1)), bottomLeftPointY * (1.0 / (m_param1)));
        glm::vec2 incrementRight = glm::vec2(bottomRightPointX * (1.0 / (m_param1)), bottomRightPointY * (1.0 / (m_param1)));

        float z;
        glm::vec3 normal;
        for (int k = 0; k < 2; k ++) {
            if (k == 0) {
                // bottom cap
                z = -0.5;
                normal = glm::vec3(0.0, -1.0, 0.0);
            } else {
                // top cap
                z = 0.5;
                normal = glm::vec3(0.0, 1.0, 0.0);
            }

            // The left and right points of the first slice of the base triangle are one increment out from the origin
            glm::vec3 left = glm::vec3(incrementLeft[0], z, incrementLeft[1]);
            glm::vec3 right = glm::vec3(incrementRight[0], z, incrementRight[1]);

            // Make the first triangle, from the origin outwards
            std::vector<glm::vec3> points;
            // Set the cap's height and normal direction
            if (k == 0) {
                // Bottom cap
                points = {right, glm::vec3(0.0, z, 0.0), left};
            } else {
                // Top cap
                points = {glm::vec3(0.0, z, 0.0), right, left};
            }

            // make the first triangle of the base
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
                std::vector<glm::vec3> points2;
                std::vector<glm::vec3> normals = {normal, normal, normal};

                // to continue building the base, we make a new tile out of two triangles

                if (k == 0) {
                    // Bottom cap
                    points1 = {left, nextLeft, right};
                    points2 = {right, nextLeft, nextRight};
                } else {
                    // Top cap
                    points1 = {nextLeft, left, right};
                    points2 = {nextLeft, right, nextRight};
                }

                // triangle 1
                // left and right connect to nextLeft
                makeTriangle(points1, normals);
                // nextLeft and right connect to nextRight
                makeTriangle(points2, normals);

                // triangle 2
                //set left to nextLeft and right to nextRight
                left = nextLeft;
                right = nextRight;
            }
        }


        // make the wall
        // make a tile up from the bottom base, increasing the vertices by one side height each time
        float increment = (1.0 / m_param1);
        for (int j = 0; j < m_param1; j++) {
            makeTile(glm::vec3(bottomRightPointX, -0.5 + (j * increment), bottomRightPointY),
                     glm::vec3(bottomLeftPointX, -0.5 + (j * increment), bottomLeftPointY),
                     glm::vec3(bottomRightPointX, -0.5  + ((j + 1) * increment), bottomRightPointY),
                     glm::vec3(bottomLeftPointX, -0.5  + ((j + 1) * increment), bottomLeftPointY)); //topL, topR, bottomL, bottomR
        }
    }
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cylinder::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
