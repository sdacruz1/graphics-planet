#include "Sphere.h"
#include "glm/ext/scalar_constants.hpp"

void Sphere::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

void Sphere::makeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    // Task 5: Implement the makeTile() function for a Sphere
    // Note: this function is very similar to the makeTile() function for Cube,
    //       but the normals are calculated in a different way!
    glm::vec3 center = {0.0, 0.0, 0.0};
    glm::vec3 TLnormal = glm::normalize(topLeft - center);
    glm::vec3 TRnormal = glm::normalize(topRight - center);
    glm::vec3 BLnormal = glm::normalize(bottomLeft - center);
    glm::vec3 BRnormal = glm::normalize(bottomRight - center);

    // multiply the point by the unique perlin value associated with its x, y ,z value (1 + that?)
    // need to modify the perlin noise evaluate function to incorporate the z value
    // topLeft = topLeft * (1 + evaluatePerlinNoise(topLeft))


    // triangle 1
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, TLnormal);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, BLnormal);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, BRnormal);

    // triangle 2
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, TLnormal);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, BRnormal);
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, TRnormal);
}

void Sphere::makeWedge(float currentTheta, float nextTheta) {
    // Task 6: create a single wedge of the sphere using the
    //         makeTile() function you implemented in Task 5
    // Note: think about how param 1 comes into play here!

    // 180 / param1 = sideLength of Tile
    // param1 = # of tiles
    // current Theta is left
    glm::vec3 topLeft;
    glm::vec3 topRight;
    glm::vec3 bottomLeft;
    glm::vec3 bottomRight;

    float phiInterval = 180.0 / m_param1;
    float x; // = 0.5 * glm::sin(phi) * glm::sin(theta);
    float y; // = 0.5 * glm::cos(phi);
    float z; // = 0.5 * glm::sin(phi) * glm::cos(theta);

    float sinCurTheta = glm::sin(nextTheta);
    float sinNextTheta = glm::sin(currentTheta);

    float cosCurTheta = glm::cos(nextTheta);
    float cosNextTheta = glm::cos(currentTheta);

    for (int i = 0; i < m_param1; i++) {
        float bottomPhi = (float)i * phiInterval;
        float topPhi = bottomPhi + phiInterval;

        float sinTopPhi = glm::sin(glm::radians(topPhi));
        float sinBottomPhi = glm::sin(glm::radians(bottomPhi));

        float cosTopPhi = glm::cos(glm::radians(topPhi));
        float cosBottomPhi = glm::cos(glm::radians(bottomPhi));

        topLeft = {0.5 * sinTopPhi * sinCurTheta, 0.5 * cosTopPhi, 0.5 * sinTopPhi * cosCurTheta};
        topRight = {0.5 * sinTopPhi * sinNextTheta, 0.5 * cosTopPhi, 0.5 * sinTopPhi * cosNextTheta};
        bottomLeft = {0.5 * sinBottomPhi * sinCurTheta, 0.5 * cosBottomPhi, 0.5 * sinBottomPhi * cosCurTheta};
        bottomRight = {0.5 * sinBottomPhi * sinNextTheta, 0.5 * cosBottomPhi, 0.5 * sinBottomPhi * cosNextTheta};

        makeTile(topLeft, topRight, bottomLeft, bottomRight);
    }
}

void Sphere::makeSphere() {
    // Task 7: create a full sphere using the makeWedge() function you
    //         implemented in Task 6
    // Note: think about how param 2 comes into play here!

    float thetaStep = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; i++) {
         float currentTheta = i * thetaStep;
         float nextTheta = currentTheta + thetaStep;
         makeWedge(currentTheta, nextTheta);
    }
}

void Sphere::setVertexData() {
    // Uncomment these lines to make a wedge for Task 6, then comment them out for Task 7:

//     float thetaStep = glm::radians(360.f / m_param2);
//     float currentTheta = 0 * thetaStep;
//     float nextTheta = 1 * thetaStep;
//     makeWedge(currentTheta, nextTheta);

    // Uncomment these lines to make sphere for Task 7:

     makeSphere();
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Sphere::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
