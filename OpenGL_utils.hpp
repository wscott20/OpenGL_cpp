#ifndef OpenGL_utils
#define OpenGL_utils
#include "glad/include/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#define GL_SILENCE_DEPRECATION
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "readFile.hpp"
#include "OpenGL_math.hpp"
using cstr = const char*;
using uint = unsigned int;
using uchar = unsigned char;
class Shader {
    uint vs,fs;
    public:
    uint program;
    int status; //if status is -1 terminate program
    Shader(string vs_path,string fs_path) {
        status = 0;
        vs = glCreateShader(GL_VERTEX_SHADER);
        fs = glCreateShader(GL_FRAGMENT_SHADER);
        //read shader files and put into strings
        string
            vs_str = readFile(vs_path),
            fs_str = readFile(fs_path);
        program = glCreateProgram();
        cstr
            vss = vs_str.c_str(),
            fss = fs_str.c_str();
        glShaderSource(vs, 1, &vss, NULL);
        glCompileShader(vs);
        glShaderSource(fs, 1, &fss, NULL);
        //compile shaders
        int vsSuccess,fsSuccess;
        glCompileShader(fs);
        glGetShaderiv(vs, GL_COMPILE_STATUS, &vsSuccess);
        glGetShaderiv(fs, GL_COMPILE_STATUS, &fsSuccess);
        if(!vsSuccess) {
            char info[512];
            glGetShaderInfoLog(vs, 512, NULL, info);
            std::cout << "Error in vertex shader\n" << info << '\n';
            status = -1;
        }
        if(!fsSuccess) {
            char info[512];
            glGetShaderInfoLog(fs, 512, NULL, info);
            std::cout << "Error in fragment shader\n" << info << '\n';
            status = -1;
        }
        glAttachShader(program,vs);
        glAttachShader(program,fs);
        glLinkProgram(program);
        int progSuccess;
        glGetProgramiv(program,GL_LINK_STATUS,&progSuccess);
        if(!progSuccess) {
            char info[512];
            glGetProgramInfoLog(program, 512, NULL, info);
            std::cout << "Error in shader program\n" << info << '\n';
            status = -1;
        }
    }
    void del() { //free up at end of program
        glDeleteShader(vs);
        glDeleteShader(fs);    
    }
    void use() {
        glUseProgram(program);
    }
    bool setUniform(cstr name, int value) {
        int loc = glGetUniformLocation(program,name);
        if (loc == -1) return false; //unsuccessful
        glUniform1i(loc,value);
        return true; //successful
    }
    bool setUniform(cstr name, float value) {
        int loc = glGetUniformLocation(program,name);
        if (loc == -1) return false;
        glUniform1f(loc,value);
        return true;
    }
    bool setUniform(cstr name, Vec2 value) {
        int loc = glGetUniformLocation(program,name);
        if (loc == -1) return false;
        glUniform2f(loc,value.x,value.y);
        return true;
    }
    bool setUniform(cstr name, Vec3 value) {
        int loc = glGetUniformLocation(program,name);
        if (loc == -1) return false;
        glUniform3f(loc,value.x,value.y,value.z);
        return true;
    }
    bool setUniform(cstr name, Mat4 value) {
        int loc = glGetUniformLocation(program,name);
        if (loc == -1) return false;
        glUniformMatrix4fv(loc,1,GL_FALSE,value.glPtr());
        return true;
    }
};
class BufferObj {
    public:
    uint vao,vbo,ebo;
    BufferObj() {
        glGenVertexArrays(1,&vao);
        glGenBuffers(1,&vbo);
        glGenBuffers(1,&ebo);
    }
    void bind() {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    }
    void data(float* verts, uint* indices, int sizeof_verts, int sizeof_indices) {
        //might have to pass in sizeof indices and verts in case its passed in as a pointer
        glBufferData(GL_ARRAY_BUFFER, sizeof_verts, verts, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof_indices, indices, GL_STATIC_DRAW);
    }
    void attrib(int index, int numElements, int vertSize, int offset) {
        glVertexAttribPointer(
            index, numElements,
            GL_FLOAT, GL_FALSE,
            vertSize * sizeof(float),
            (void*)(offset * sizeof(float))
        );
        glEnableVertexAttribArray(index);
    }
    void updateV(int offset, int size, float* data) {
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(float), size * sizeof(float), data);
    }
};
class Texture {
    uint tex;
    cstr locName;
    public:
    Texture(cstr name="tex"): locName{name} {
        glGenTextures(1, &tex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
    }
    void data(Shader shader) {
        shader.setUniform(locName,0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    uchar* loadTex(cstr filename) {
        int w,h,chan;
        uchar *data = stbi_load(filename, &w, &h, &chan, 0);
        if (!data) std::cout << "error loading data\n";
        else std::cout << "Texture loaded: " << w << "x" << h << " with " << chan << " channels\n";
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        return data;
    }
    void loadData(cstr filename) {
        uchar *texData = loadTex(filename);
        if (texData) stbi_image_free(texData);
    }
};
struct Mesh {
    std::vector<Vec3> positions,colors,normals;
    std::vector<Vec2> texcoords;
    std::vector<uint> indices;
    int vCount,iCount; //vertex count and index count
    Mesh(int vertexCount,int indexCount): vCount{vertexCount}, iCount{indexCount} {
        //initializes vec values to 0
        positions = std::vector<Vec3>(vCount);
        colors = std::vector<Vec3>(vCount);
        normals = std::vector<Vec3>(vCount);
        texcoords = std::vector<Vec2>(vCount);
        indices = std::vector<uint>(iCount);
    }
    void setPositions(std::vector<Vec3> coords) {
        for (int i = 0; i < vCount; i++)
            positions[i]=coords[i];
    }
    void setColors(std::vector<Vec3> cols) {
        for (int i = 0; i < vCount; i++)
            colors[i]=cols[i];
    }
    void setNorms(std::vector<Vec3> norms) {
        for (int i = 0; i < vCount; i++)
            normals[i]=norms[i];
    }
    void setTexCoords(std::vector<Vec2> coords) {
        for (int i = 0; i < vCount; i++)
            texcoords[i]=coords[i];
    }
    void setIndices(std::vector<uint> indexes) {
        for (int i = 0; i < iCount; i++)
            indices[i]=indexes[i];
    }
    std::vector<float> vData(BufferObj bo, bool attrib=true) {
        //sets buffer data
        std::vector<float> data = {};
        for (int i = 0; i < vCount; i++) {
            data.insert(data.end(),{positions[i].x,positions[i].y,positions[i].z});
            data.insert(data.end(),{colors[i].x,colors[i].y,colors[i].z});
            data.insert(data.end(),{normals[i].x,normals[i].y,normals[i].z});
            data.insert(data.end(),{texcoords[i].x,texcoords[i].y});
        }
        float* verts = data.data();
        bo.data(verts,indices.data(),vCount * 11 * sizeof(float),iCount*sizeof(uint));
        //sometimes might have to sometimes set buffer data multiple times but only attrib once
        if (attrib) {
            bo.attrib(0,3,11,0); //11 for vec3 position vec3 color vec3 normal vec2 texcoord
            bo.attrib(1,3,11,3);
            bo.attrib(2,3,11,6);
            bo.attrib(3,2,11,9);
        }
        return data;
    }
};
Mesh genCube(BufferObj bo, float width, float height, float length, Vec3 color={1,1,1}) {
    std::vector<Vec3> positions = {
        {-.5,  .5,  .5},
        {-.5, -.5,  .5},
        { .5, -.5,  .5},
        { .5,  .5,  .5},
        { .5,  .5, -.5},
        { .5, -.5, -.5},
        {-.5, -.5, -.5},
        {-.5,  .5, -.5},
        { .5,  .5,  .5},
        { .5,  .5, -.5},
        {-.5,  .5, -.5},
        {-.5,  .5,  .5},
        { .5, -.5,  .5},
        { .5, -.5, -.5},
        {-.5, -.5, -.5},
        {-.5, -.5,  .5},
        { .5,  .5,  .5},
        { .5, -.5,  .5},
        { .5, -.5, -.5},
        { .5,  .5, -.5},
        {-.5,  .5, -.5},
        {-.5, -.5, -.5},
        {-.5, -.5,  .5},
        {-.5,  .5,  .5}
    };
    for(auto& pos: positions) pos *= Vec3{width,height,length};
    Vec3 initNorms[] = {
        {0,0,1},
        {0,0,-1},
        {0,1,0},
        {0,-1,0},
        {1,0,0},
        {-1,0,0}
    };
    std::vector<Vec3> norms(24);
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 4; j++)
            norms[i*4+j] = initNorms[i];
    std::vector<Vec3> colors(24,color);
    Vec2 initTexcoords[] = {
        {1,1},
        {1,0},
        {0,0},
        {0,1}
    };
    std::vector<Vec2> texcoords(24);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 6; j++)
            texcoords[j*4+i] = initTexcoords[i];
    std::vector<uint> indices(36);
    for (int i = 0; i < 36; i+=6) {
        uint faceInd[] = {0,1,2,0,2,3};
        for (int j = 0; j < 6; j++)
            indices[i+j] = faceInd[j] + (4 * i/6);
    }
    Mesh cube{24,36};
    cube.setPositions(positions);
    cube.setNorms(norms);
    cube.setIndices(indices);
    cube.setColors(colors);
    cube.setTexCoords(texcoords);
    cube.vData(bo);
    return cube;
}
Mesh genSphere(BufferObj bo, float radius, int latLines, int lonLines, Vec3 color=Vec3{1,1,1}) {
    std::vector<Vec3> positions={};
    std::vector<Vec3> colors={};
    std::vector<Vec3> norms={};
    std::vector<Vec2> texcoords={};
    std::vector<uint> indices={};
    Vec3 og{0,0,radius}; //original vector to be rotated
    int index=0;
    for (int i = 0; i < latLines; i++) {
        for (int j = 0; j < lonLines; j++) {
            float latAngle = 180/latLines * i - 90,
                  nextLat  = 180/latLines * (i+1) - 90,
                  lonAngle = 360/lonLines * j,
                  nextLon  = 360/lonLines * (j+1);
            if (i == 0) {
                Vec3 p1 = Mat4Vec3(Mat4::rotate(lonAngle,Y,Deg) * Mat4::rotate(latAngle,X,Deg) * Mat4::translate(og)),
                     p2 = Mat4Vec3(Mat4::rotate(lonAngle,Y,Deg) * Mat4::rotate(nextLat,X,Deg) * Mat4::translate(og)),
                     p3 = Mat4Vec3(Mat4::rotate(nextLon,Y,Deg) * Mat4::rotate(nextLat,X,Deg) * Mat4::translate(og)),
                     norm = Vec3::cross(p1-p2,p1-p3);
                positions.push_back(p1);
                positions.push_back(p2);
                positions.push_back(p3);
                uint inds[] = {0,1,2};
                for (uint ind: inds) {
                    indices.push_back(ind + index);
                    colors.push_back(color);
                    norms.push_back(norm.normalize());
                }
                texcoords.push_back({.5,1}); //idk what else to use for texcoords on just a triangle
                texcoords.push_back({0,0});
                texcoords.push_back({0,1});
                index += 3;
            } else if (i == latLines-1) {
                Vec3 p1 = Mat4Vec3(Mat4::rotate(lonAngle,Y,Deg) * Mat4::rotate(latAngle,X,Deg) * Mat4::translate(og)),
                     p2 = Mat4Vec3(Mat4::rotate(lonAngle,Y,Deg) * Mat4::rotate(nextLat,X,Deg) * Mat4::translate(og)),
                     p3 = Mat4Vec3(Mat4::rotate(nextLon,Y,Deg) * Mat4::rotate(latAngle,X,Deg) * Mat4::translate(og)),
                     norm = Vec3::cross(p1-p2,p1-p3);
                positions.push_back(p1);
                positions.push_back(p2);
                positions.push_back(p3);
                uint inds[] = {0,1,2};
                for (uint ind: inds) {
                    indices.push_back(ind + index);
                    colors.push_back(color);
                    norms.push_back(norm.normalize());
                }
                texcoords.push_back({1,0});
                texcoords.push_back({.5,0});
                texcoords.push_back({1,1});
                index += 3;
            } else {
                Vec3 p1 = Mat4Vec3(Mat4::rotate(lonAngle,Y,Deg) * Mat4::rotate(latAngle,X,Deg) * Mat4::translate(og)),
                     p2 = Mat4Vec3(Mat4::rotate(lonAngle,Y,Deg) * Mat4::rotate(nextLat,X,Deg) * Mat4::translate(og)),
                     p3 = Mat4Vec3(Mat4::rotate(nextLon,Y,Deg) * Mat4::rotate(nextLat,X,Deg) * Mat4::translate(og)),
                     p4 = Mat4Vec3(Mat4::rotate(nextLon,Y,Deg) * Mat4::rotate(latAngle,X,Deg) * Mat4::translate(og)),
                     norm = Vec3::cross(p1-p2,p1-p3);
                positions.push_back(p1);
                positions.push_back(p2);
                positions.push_back(p3);
                positions.push_back(p4);
                uint inds[] = {0,1,2,0,2,3};
                for (uint ind: inds)
                    indices.push_back(ind + index);
                for (int k = 0; k < 4; k++) {
                    colors.push_back(color);
                    norms.push_back(norm.normalize());
                }
                texcoords.push_back({1,0});
                texcoords.push_back({0,0});
                texcoords.push_back({0,1});
                texcoords.push_back({1,1});
                index += 4;
            }
        }
    }
    Mesh sphere{(int)positions.size(),(int)indices.size()};
    sphere.setPositions(positions);
    sphere.setColors(colors);
    sphere.setNorms(norms);
    sphere.setTexCoords(texcoords);
    sphere.setIndices(indices);
    sphere.vData(bo);
    return sphere;
}
struct Model {
    cstr locName;
    std::vector<Mesh> meshes;
    Mat4 transform;
    Texture texture;
    Shader shader;
    Model(Shader s,std::vector<Mesh> meshArr,Mat4 transformMat,cstr name): shader{s}, meshes{meshArr}, transform{transformMat}, locName{name} {
        shader.setUniform(locName,transform);
    }
    void setTexture(cstr filename) {
        texture.data(shader);
        texture.loadData(filename);
    }
    void updateTransform() {
        shader.setUniform(locName,transform);
    }
    void draw(BufferObj bo, bool multi=false) {
        updateTransform();
        for (auto mesh: meshes) {
            if(multi) mesh.vData(bo,false); //might have to update buffer data when drawing multiple meshes/models
            glDrawElements(GL_TRIANGLES, mesh.iCount, GL_UNSIGNED_INT, 0);
        }
    }
};
#endif