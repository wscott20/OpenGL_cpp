#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "model.hpp"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
uint loadTexture(const char* path, GLenum format, bool srgb) {
    uint tex;
    glGenTextures(1, &tex);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        if (format == GL_NONE) {
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;
        }
        GLenum internalFormat = srgb ? (
            format == GL_RGB   ? GL_SRGB :
            format == GL_RGBA  ? GL_SRGB_ALPHA :
            format
        ) : format;
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    } else {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
        return 0;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return tex;
}
Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<glm::vec3> verts;
    std::vector<glm::vec3> norms;
    std::vector<glm::vec2> uvs;
    std::vector<uint> indices;
    for (int i = 0; i < mesh->mNumVertices; i++) {
        glm::vec3 position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        glm::vec3 normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        glm::vec2 uv;
        if (mesh->mTextureCoords[0])
            uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else uv = glm::vec2(0.0f, 0.0f);
        verts.push_back(position);
        norms.push_back(normal);
        uvs.push_back(uv);
    }
    for(int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    Material mat;
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString str;
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS) {
            glActiveTexture(GL_TEXTURE0);
            mat.diffuse = loadTexture(str.C_Str());
        }
        if (material->GetTexture(aiTextureType_SPECULAR, 0, &str) == AI_SUCCESS) {
            glActiveTexture(GL_TEXTURE1);
            mat.specular = loadTexture(str.C_Str());
        }
        material->Get(AI_MATKEY_SHININESS, mat.shininess);
    }
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
    Mesh m;
    m.vertices = verts;
    m.normals = norms;
    m.uvs = uvs;
    m.tangents = tans;
    //m.bitangents = bitans;
    m.indices = indices;
    m.uploadMesh();
    return m;
}
void processNode(aiNode* node, const aiScene* scene, Model& model) {
    for(int i = 0; i < node->mNumMeshes; i++){
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        model.meshes.push_back(processMesh(mesh, scene));			
    }
    for(int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene, model);
}
Model loadModel(const char* path) {
    Model model;
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); 
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Assimp Error\n" << importer.GetErrorString() << std::endl;
        return model;
    }
    processNode(scene->mRootNode, scene, model);
    return model;
}
void Model::draw() {
    for (auto& mesh : meshes) mesh.draw();
}