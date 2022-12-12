#include "Cube.h"

void Cube::updateParams(int param1) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    setVertexData();
    makeCube();
}

void Cube::makeCube() {
    // Generate random vector lookup table
    m_lookupSize = 1024;
    m_randVecLookup.reserve(m_lookupSize);

    // Initialize random number generator
    std::srand(1230);

    // Populate random vector lookup table
    for (int i = 0; i < m_lookupSize; i++)
    {
      m_randVecLookup.push_back(glm::vec2(std::rand() * 2.0 / RAND_MAX - 1.0,
                                          std::rand() * 2.0 / RAND_MAX - 1.0));
      }
}

// Helper for computePerlin() and, possibly, getColor()
float interpolate(float A, float B, float alpha) {
    // Task 4: implement your easing/interpolation function below
    float ease = 3 * pow(alpha, 2) - 2 * pow(alpha, 3);

    return A + ease * (B - A);
}

// Samples the (infinite) random vector grid at (row, col)
glm::vec2 Cube::sampleRandomVector(int row, int col)
{
    std::hash<int> intHash;
    int index = intHash(row * 41 + col * 43) % m_lookupSize;
    return m_randVecLookup.at(index);
}

// Computes the intensity of Perlin noise at some point
float Cube::computePerlin(float x, float y) {
    // Task 1: get grid indices (as ints)
    int X = (int)x;
    int Y = (int)y;
    glm::vec2 i1 = {X, Y};
    glm::vec2 i2 = {X + 1, Y};
    glm::vec2 i3 = {X, Y + 1};
    glm::vec2 i4 = {X + 1, Y + 1};

    // Task 2: compute offset vectors
    glm::vec2 input = {x,y};
    glm::vec2 off1 = input - i1;
    glm::vec2 off2 = input - i2;
    glm::vec2 off3 = input - i3;
    glm::vec2 off4 = input - i4;

    // Task 3: compute the dot product between offset and grid vectors
    float dot1 = glm::dot(off1, sampleRandomVector(i1[0], i1[1]));
    float dot2 = glm::dot(off2, sampleRandomVector(i2[0], i2[1]));
    float dot3 = glm::dot(off3, sampleRandomVector(i3[0], i3[1]));
    float dot4 = glm::dot(off4, sampleRandomVector(i4[0], i4[1]));

    // Task 5: use your interpolation function to produce the final value
    float inter1 = interpolate(dot1, dot2, x - i1[0]);
    float inter2 = interpolate(dot3, dot4, x - i1[0]);

    // Return 0 as a placeholder
    return interpolate(inter1, inter2, y - i1[1]);
}

// Takes a normalized (x, y) position, in range [0,1)
// Returns a height value, z, by sampling a noise function
float Cube::getHeight(float x, float y) {

    // Task 6: modify this call to produce noise of a different frequency
    //float z = computePerlin(x * 8, y * 8) / 8;
    float z = 0;

    // Task 7: combine multiple different octaves of noise to produce fractal perlin noise
    z += computePerlin(x * 8, y * 8) / 8;
    z += computePerlin(x * 16, y * 16) / 16;
    z += computePerlin(x * 32, y * 32) / 32;
    z += computePerlin(x * 64, y * 64) / 64;

    // Return 0 as placeholder
    return z;
}

///////////////////////////***** BASE *****/////////////////////////
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
    float Val;// = 0.5f;


    float sideLength = 1.0f / m_param1;
    for (int x = 0; x < m_param1; x ++) {
        xVal = -0.5f + (x * sideLength);
        xValLong = xVal + sideLength;

        for (int y = 0; y < m_param1; y ++) {
            yVal = -0.5f + (y * sideLength);
            yValLong = yVal + sideLength;

            Val = getHeight(xVal, yValLong);
            topLeft = {xVal, yValLong, Val};

            Val = getHeight(xValLong, yValLong);
            topRight = {xValLong, yValLong, Val};

            Val = getHeight(xVal, yVal);
            bottomLeft = {xVal, yVal, Val};

            Val = getHeight(xValLong, yVal);
            bottomRight = {xValLong, yVal, Val};


            makeTile(topLeft, topRight, bottomLeft, bottomRight);
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
