#pragma once
#include "mesh.hpp"
struct Model {
    std::vector<Mesh> meshes;
    void draw();
};
uint loadTexture(const char* path, GLenum format = GL_NONE, bool srgb = false);
Model loadModel(const char* path);