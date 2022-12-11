#include "Cube.h"

void Cube::updateParams(int param1) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    setVertexData();
}

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 2: create a tile (i.e. 2 triangles) based on 4 given points.

    //normals are destination - source
    glm::vec3 TLnormal = glm::normalize(glm::cross(bottomLeft - topLeft, topRight - topLeft));
    glm::vec3 TRnormal = glm::normalize(glm::cross(topLeft - topRight, bottomRight - topRight));
    glm::vec3 BLnormal = glm::normalize(glm::cross(bottomRight - bottomLeft, topLeft - bottomLeft));
    glm::vec3 BRnormal = glm::normalize(glm::cross(topRight - bottomRight, bottomLeft - bottomRight));

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

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 3: create a single side of the cube out of the 4
    //         given points and makeTile()
    // Note: think about how param 1 affects the number of triangles on
    //       the face of the cube

    // 2 * (param1^2) triangles
    // 1 / (param1^2) = side length of one triangle
    // bottomLeft is 0, 0 in row, col style

    float xVal;
    float xValLong;
    float yVal;
    float yValLong;
    float Val;

    float sideLength = 1.0f / m_param1;
    for (int i = 0; i < 6; i++) {
    for (int x = 0; x < m_param1; x ++) {
        xVal = -0.5f + (x * sideLength);
        xValLong = xVal + sideLength;

        for (int y = 0; y < m_param1; y ++) {
                yVal = -0.5f + (y * sideLength);
                yValLong = yVal + sideLength;

            if (i == 0) {
                Val = 0.5f;

                topLeft = {xVal, yValLong, Val};
                topRight = {xValLong, yValLong, Val};
                bottomLeft = {xVal, yVal, Val};
                bottomRight = {xValLong, yVal, Val};

            } else if (i ==1) {
                Val = -0.5f;

                topRight = {xVal, yValLong, Val};
                topLeft = {xValLong, yValLong, Val};
                bottomRight = {xVal, yVal, Val};
                bottomLeft = {xValLong, yVal, Val};

            } else if (i == 2) {
                Val = 0.5f;

                topRight = {xVal, Val, yValLong};
                topLeft = {xValLong, Val, yValLong};
                bottomRight = {xVal, Val, yVal};
                bottomLeft = {xValLong, Val, yVal};

            } else if (i == 3) {
                Val = -0.5f;

                topLeft = {xVal, Val, yValLong};
                topRight = {xValLong, Val, yValLong};
                bottomLeft = {xVal, Val, yVal};
                bottomRight = {xValLong, Val, yVal};

            } else if (i == 4) {
                Val = 0.5f;

                topLeft = {Val, xVal, yValLong};
                topRight = {Val, xValLong, yValLong};
                bottomLeft = {Val, xVal, yVal};
                bottomRight = {Val, xValLong, yVal};
            } else if (i == 5) {
                Val = -0.5f;

                topRight = {Val, xVal, yValLong};
                topLeft = {Val, xValLong, yValLong};
                bottomRight = {Val, xVal, yVal};
                bottomLeft = {Val, xValLong, yVal};
            }

            makeTile(topLeft, topRight, bottomLeft, bottomRight);
        }
    }
    }

}

void Cube::setVertexData() {
    // Uncomment these lines for Task 2, then comment them out for Task 3:

//     makeTile(glm::vec3(-0.5f,  0.5f, 0.5f),
//              glm::vec3( 0.5f,  0.5f, 0.5f),
//              glm::vec3(-0.5f, -0.5f, 0.5f),
//              glm::vec3( 0.5f, -0.5f, 0.5f));

    // Uncomment these lines for Task 3:

     makeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
              glm::vec3( 0.5f,  0.5f, 0.5f),
              glm::vec3(-0.5f, -0.5f, 0.5f),
              glm::vec3( 0.5f, -0.5f, 0.5f));

    // Task 4: Use the makeFace() function to make all 6 sides of the cube
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cube::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
