#include "mesh.hpp"
#include <iostream>
#include <glad/glad.h>
Mesh::~Mesh() {
    std::cout << "freeing buffers" << std::endl;
    glDeleteVertexArrays(1,&vao);
    glDeleteBuffers(4,//5,
        vbo);
    glDeleteBuffers(1,&ebo);
}
void Mesh::uploadMesh(AttribLocMap locs) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(4,//5,
         vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(locs.pos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(locs.pos);
    if (!normals.empty() && locs.norms != -1) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(locs.norms, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(locs.norms);
    }
    if (!uvs.empty() && locs.uvs != -1) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
        glVertexAttribPointer(locs.uvs, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(locs.uvs);
    }
    if (!tangents.empty() && locs.tans != -1) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
        glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec4), &tangents[0], GL_STATIC_DRAW);
        glVertexAttribPointer(locs.tans, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(locs.tans);
    }
    /*if (!bitangents.empty() && locs.bitans != -1) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
        glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), &bitangents[0], GL_STATIC_DRAW);
        glVertexAttribPointer(locs.bitans, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(locs.bitans);
    }*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
}
void Mesh::setupMaterial(Shader shader) {
    material.locs[DIFFUSE_LOC] = shader.getLoc("material.diffuse");
    shader.setInt(material.locs[DIFFUSE_LOC], 0);
    material.locs[SPECULAR_LOC] = shader.getLoc("material.specular");
    shader.setInt(material.locs[SPECULAR_LOC], 1);
    material.locs[SHININESS_LOC] = shader.getLoc("material.shininess");
    shader.setFloat(material.locs[SHININESS_LOC], material.shininess);
}
void Mesh::draw(GLenum primType) {
    glBindVertexArray(vao);
    glDrawElements(primType, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
Mesh genCube(float width, float height, float depth, AttribLocMap locs) {
    std::vector<glm::vec3> verts = {
        //front face
        {width/2, -height/2, depth/2},
        {width/2, height/2, depth/2},
        {-width/2, height/2, depth/2},
        {-width/2, -height/2, depth/2},
        //back face
        {-width/2, -height/2, -depth/2},
        {-width/2, height/2, -depth/2},
        {width/2, height/2, -depth/2},
        {width/2, -height/2, -depth/2},
        //top face
        {width/2, height/2, -depth/2},
        {width/2, height/2, depth/2},
        {-width/2, height/2, depth/2},
        {-width/2, height/2, -depth/2},
        //bottom face
        {-width/2, -height/2, -depth/2},
        {-width/2, -height/2, depth/2},
        {width/2, -height/2, depth/2},
        {width/2, -height/2, -depth/2},
        //right face
        {width/2, -height/2, depth/2},
        {width/2, height/2, depth/2},
        {width/2, height/2, -depth/2},
        {width/2, -height/2, -depth/2},
        //left face
        {-width/2, -height/2, -depth/2},
        {-width/2, height/2, -depth/2},
        {-width/2, height/2, depth/2},
        {-width/2, -height/2, depth/2},
    };
    std::vector<glm::vec3> norms = {
        //front
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        //back
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        //top
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        //bottom
        {0.0f, -1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        //right
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        //left
        {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
    };
    std::vector<glm::vec2> uvs = {
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},
        
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},
        
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},
        
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},
        
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},
        
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},
    };
    std::vector<uint> indices = {
        0,  1,  2,  0,  2,  3,  //front
        4,  5,  6,  4,  6,  7,  //back
        8,  9,  10, 8,  10, 11, //top
        12, 13, 14, 12, 14, 15, //bottom
        16, 17, 18, 16, 18, 19, //right
        20, 21, 22, 20, 22, 23, //left
    };
    std::vector<glm::vec4> tans(verts.size());
    std::vector<glm::vec3>
        tanAccum(verts.size()),
        bitanAccum(verts.size());
    for (int i = 0; i < indices.size(); i+=3) {
        uint i1=indices[i], i2=indices[i+1], i3=indices[i+2];
        glm::vec3 edge0 = verts[i2]-verts[i1];
        glm::vec3 edge1 = verts[i3]-verts[i1];
        glm::vec2 deltauv0 = uvs[i2]-uvs[i1];
        glm::vec2 deltauv1 = uvs[i3]-uvs[i1];
        float det = deltauv0.x*deltauv1.y - deltauv1.x*deltauv0.y;
        if (fabs(det) < 1e-6f) det = 1e-6f;
        float f = 1.f / det;
        glm::vec3 tan = f * (deltauv1.y*edge0 - deltauv0.y*edge1);
        tanAccum[i1] += tan;
        tanAccum[i2] += tan;
        tanAccum[i3] += tan;
        glm::vec3 bitan = f * (deltauv0.x*edge1 - deltauv1.x*edge0);
        bitanAccum[i1] += bitan;
        bitanAccum[i2] += bitan;
        bitanAccum[i3] += bitan;
    }
    for (int i = 0; i < tanAccum.size(); i++) {
        auto
            t = glm::normalize(tanAccum[i]),
            b = bitanAccum[i],
            n = norms[i];
        t = glm::normalize(t - n * glm::dot(n, t));
        float handedness = glm::dot(glm::cross(n, t), b) < 0 ? -1 : 1;
        tans[i] = glm::vec4(t, handedness);
    }
    Mesh mesh;
    mesh.vertices = verts;
    mesh.normals = norms;
    mesh.uvs = uvs;
    mesh.tangents = tans;
    //mesh.bitangents = bitans;
    mesh.indices = indices;
    mesh.uploadMesh();
    return mesh;
}