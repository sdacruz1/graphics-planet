#include "Triangle.h"

void Triangle::updateParams() {
    m_vertexData = std::vector<float>();
    setVertexData();
}

void Triangle::setVertexData() {
    // Task 1: update m_vertexData with the vertices and normals
    //         needed to tesselate a triangle
    // Note: you may find the insertVec3 function (in the OpenGLShape class
    //       useful in adding your points into m_vertexData
    glm::vec3 normal = {0.0, 0.0, 1.0};

    glm::vec3 top = {-0.5, 0.5, 0.0};
    glm::vec3 bottomLeft = {-0.5, -0.5, 0.0};
    glm::vec3 bottomRight = {0.5, -0.5, 0.0};

    glm::vec3 Tnormal = normal; //glm::normalize(glm::cross(bottomRight - top, bottomLeft - top));
    glm::vec3 BLnormal = normal; //glm::normalize(glm::cross(top - bottomLeft, bottomRight - bottomLeft));
    glm::vec3 BRnormal = normal; //glm::normalize(glm::cross(top - bottomRight, bottomLeft - bottomRight));

    insertVec3(m_vertexData, top);
    insertVec3(m_vertexData, Tnormal);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, BLnormal);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, BRnormal);
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Triangle::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
