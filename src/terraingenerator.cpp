#include "terraingenerator.h"

#include <cmath>
#include "glm/glm.hpp"

// Constructor
TerrainGenerator::TerrainGenerator()
{
  // Task 8: turn off wireframe shading
  m_wireshade = false; // STENCIL CODE
  // m_wireshade = false; // TA SOLUTION

  // Define resolution of terrain generation
  m_resolution = 100;

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

// Destructor
TerrainGenerator::~TerrainGenerator()
{
    m_randVecLookup.clear();
}

// Helper for generateTerrain()
void addPointToVector(glm::vec3 point, std::vector<float>& vector) {
    vector.push_back(point.x);
    vector.push_back(point.y);
    vector.push_back(point.z);
}

// Generates the geometry of the output triangle mesh
std::vector<float> TerrainGenerator::generateTerrain() {
    std::vector<float> verts;
    verts.reserve(m_resolution * m_resolution * 6);

    for(int x = 0; x < m_resolution; x++) {
        for(int y = 0; y < m_resolution; y++) {
            int x1 = x;
            int y1 = y;

            int x2 = x + 1;
            int y2 = y + 1;

            glm::vec3 p1 = getPosition(x1,y1);
            glm::vec3 p2 = getPosition(x2,y1);
            glm::vec3 p3 = getPosition(x2,y2);
            glm::vec3 p4 = getPosition(x1,y2);

            glm::vec3 n1 = getNormal(x1,y1);
            glm::vec3 n2 = getNormal(x2,y1);
            glm::vec3 n3 = getNormal(x2,y2);
            glm::vec3 n4 = getNormal(x1,y2);

            // tris 1
            // x1y1z1
            // x2y1z2
            // x2y2z3
            addPointToVector(p1, verts);
            addPointToVector(n1, verts);
            addPointToVector(getColor(n1, p1), verts);

            addPointToVector(p2, verts);
            addPointToVector(n2, verts);
            addPointToVector(getColor(n2, p2), verts);

            addPointToVector(p3, verts);
            addPointToVector(n3, verts);
            addPointToVector(getColor(n3, p3), verts);

            // tris 2
            // x1y1z1
            // x2y2z3
            // x1y2z4
            addPointToVector(p1, verts);
            addPointToVector(n1, verts);
            addPointToVector(getColor(n1, p1), verts);

            addPointToVector(p3, verts);
            addPointToVector(n3, verts);
            addPointToVector(getColor(n3, p3), verts);

            addPointToVector(p4, verts);
            addPointToVector(n4, verts);
            addPointToVector(getColor(n4, p4), verts);
        }
    }
    return verts;
}

// Samples the (infinite) random vector grid at (row, col)
glm::vec2 TerrainGenerator::sampleRandomVector(int row, int col)
{
    std::hash<int> intHash;
    int index = intHash(row * 41 + col * 43) % m_lookupSize;
    return m_randVecLookup.at(index);
}

// Takes a grid coordinate (row, col), [0, m_resolution), which describes a vertex in a plane mesh
// Returns a normalized position (x, y, z); x and y in range from [0, 1), and z is obtained from getHeight()
glm::vec3 TerrainGenerator::getPosition(int row, int col) {
    // Normalizing the planar coordinates to a unit square 
    // makes scaling independent of sampling resolution.
    float x = 1.0 * row / m_resolution;
    float y = 1.0 * col / m_resolution;
    float z = getHeight(x, y);
    return glm::vec3(x,y,z);
}

// ================== Students, please focus on the code below this point

// Helper for computePerlin() and, possibly, getColor()
float interpolate(float A, float B, float alpha) {
    // Task 4: implement your easing/interpolation function below
    float ease = 3 * pow(alpha, 2) - 2 * pow(alpha, 3);

    return A + ease * (B - A);
}

// Takes a normalized (x, y) position, in range [0,1)
// Returns a height value, z, by sampling a noise function
float TerrainGenerator::getHeight(float x, float y) {

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

// Computes the normal of a vertex by averaging neighbors
glm::vec3 TerrainGenerator::getNormal(int row, int col) {
    // Task 9: Compute the average normal for the given input indices
    glm::vec3 Vpos = getPosition(row, col);

    glm::vec2 n0 = {row - 1, col - 1};
    glm::vec2 n1 = {row, col - 1};
    glm::vec2 n2 = {row + 1, col - 1};
    glm::vec2 n3 = {row + 1, col};
    glm::vec2 n4 = {row + 1, col + 1};
    glm::vec2 n5 = {row, col + 1};
    glm::vec2 n6 = {row - 1, col + 1};
    glm::vec2 n7 = {row - 1, col};

    std::vector nS = {n0, n1, n2, n3, n4, n5, n6, n7};
    glm::vec3 normalS = {0, 0, 0};

    for (int i = 0; i < 8; i++) {
        glm::vec3 Vni = getPosition(nS[i][0], nS[i][1]) - Vpos;
        glm::vec3 VniPlus1 = getPosition(nS[i + 1][0], nS[i + 1][1]) - Vpos;
        glm::vec3 normal = glm::cross(Vni, VniPlus1);
        normalS += glm::normalize(normal);
    }

    normalS = glm::normalize(normalS);

    // Return up as placeholder
    return normalS;
//    return glm::vec3(0,0,1);
}

// Computes color of vertex using normal and, optionally, position
glm::vec3 TerrainGenerator::getColor(glm::vec3 normal, glm::vec3 position) {
    // Task 10: compute color as a function of the normal and position
    float zThreshold = 0.002;
    float dirThreshold = 2;
    if ((position[2] >= zThreshold) && (glm::dot(normal, glm::vec3{0, 0, 1}) <= dirThreshold)) {
        return glm::vec3(1, 1, 1);
    }

    // Return white as placeholder
    return glm::vec3(0.5,0.5,0.5);
}

// Computes the intensity of Perlin noise at some point
float TerrainGenerator::computePerlin(float x, float y) {
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
