#pragma once
#include <glm/glm.hpp>
#include "material.hpp"
#include <vector>
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};
struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    Material material;
    uint vao, vbo, ebo;
    Mesh(std::vector<Vertex> verts, std::vector<uint> inds);
    Mesh(std::vector<Vertex> verts, std::vector<uint> inds, Material mat);
    void setupMesh(uint shaderProgram);
    void draw();
};
Mesh genCube(float width, float height, float depth);
//will implement later
//Mesh genCubeUV(float width, float height, float depth, std::vector<glm::vec2> uvs);