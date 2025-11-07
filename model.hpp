#pragma once
#include "mesh.hpp"
struct Model {
    std::vector<Mesh> meshes;
    void draw();
};
uint loadTexture(const char* path);
Model loadModel(const char* path);