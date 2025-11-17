#pragma once
#include <glm/glm.hpp>
#include "material.hpp"
#include "shader.hpp"
#include <vector>
struct AttribLocMap {
    int pos, norms, uvs, tans; //, bitans;
};
constexpr AttribLocMap defAttrLocs{0, 1, 2, 3/*, 4*/};
struct Mesh {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec4> tangents;
    //std::vector<glm::vec3> bitangents;
    std::vector<uint> indices;
    Material material;
    uint vao, vbo[4], ebo; //using 4 for now not using bitangents on cpu yet
    //uint vao, vbo[5], ebo; //5 vbos for options for positions uvs normals tangents
    void uploadMesh(AttribLocMap locs=defAttrLocs);
    void setupMaterial(Shader shader);
    void draw();
    ~Mesh();
};
Mesh genCube(float width, float height, float depth, AttribLocMap locs=defAttrLocs);