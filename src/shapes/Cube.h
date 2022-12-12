#pragma once

#include <vector>
#include <glm/glm.hpp>

class Cube
{
public:
    void updateParams(int param1);
    std::vector<float> generateShape() { return m_vertexData; }
    std::vector<glm::vec2> m_randVecLookup;
    int m_lookupSize;

    glm::vec2 sampleRandomVector(int row, int col);
    float computePerlin(float x, float y);
    float getHeight(float x, float y);
    void makeCube();
    float interpolate(float A, float B, float alpha);

private:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void setVertexData();
    void makeTile(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);
    void makeFace(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);

    std::vector<float> m_vertexData;
    int m_param1;
};
