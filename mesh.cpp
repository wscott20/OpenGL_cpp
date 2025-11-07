#include "mesh.hpp"
#include <glad/glad.h>
// #include <assimp/Importer.hpp>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>
Mesh::Mesh(std::vector<Vertex> verts, std::vector<uint> inds)
: vertices(verts), indices(inds) {}
Mesh::Mesh(std::vector<Vertex> verts, std::vector<uint> inds, Material mat)
: vertices(verts), indices(inds), material(mat) {}
void Mesh::setupMesh(uint shaderProgram) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);
    //glBindVertexArray(0);
    material.locs[DIFFUSE_LOC] = glGetUniformLocation(shaderProgram, "material.diffuse");
    material.locs[SPECULAR_LOC] = glGetUniformLocation(shaderProgram, "material.specular");
    material.locs[SHININESS_LOC] = glGetUniformLocation(shaderProgram, "material.shininess");
    glUniform1i(material.locs[DIFFUSE_LOC], 0);
    glUniform1i(material.locs[SPECULAR_LOC], 1);
    glUniform1f(material.locs[SHININESS_LOC], material.shininess);
}
void Mesh::draw() {
    /*glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.diffuse);
    glUniform1i(material.locs[DIFFUSE_LOC], 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, material.specular);
    glUniform1i(material.locs[SPECULAR_LOC], 1);
    glUniform1f(material.locs[SHININESS_LOC], material.shininess);
    glActiveTexture(GL_TEXTURE0);*/
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
}
Mesh genCube(float width, float height, float depth) {
    std::vector<Vertex> vertices = {
        //front face
        {{width/2, -height/2, depth/2}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{width/2, height/2, depth/2}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-width/2, height/2, depth/2}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{-width/2, -height/2, depth/2}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        //back face
        {{-width/2, -height/2, -depth/2}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
        {{-width/2, height/2, -depth/2}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{width/2, height/2, -depth/2}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        {{width/2, -height/2, -depth/2}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        //top face
        {{width/2, height/2, -depth/2}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{width/2, height/2, depth/2}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-width/2, height/2, depth/2}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{-width/2, height/2, -depth/2}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        //bottom face
        {{-width/2, -height/2, -depth/2}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        {{-width/2, -height/2, depth/2}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{width/2, -height/2, depth/2}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
        {{width/2, -height/2, -depth/2}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        //right face
        {{width/2, -height/2, depth/2}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{width/2, height/2, depth/2}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{width/2, height/2, -depth/2}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{width/2, -height/2, -depth/2}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        //left face
        {{-width/2, -height/2, -depth/2}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-width/2, height/2, -depth/2}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-width/2, height/2, depth/2}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{-width/2, -height/2, depth/2}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    };
    std::vector<uint> indices = {
         0,  1,  2,  0,  2,  3, //front
         4,  5,  6,  4,  6,  7, //back
         8,  9, 10,  8, 10, 11, //top
        12, 13, 14, 12, 14, 15, //bottom
        16, 17, 18, 16, 18, 19, //right
        20, 21, 22, 20, 22, 23  //left
    };
    return Mesh(vertices, indices);
}