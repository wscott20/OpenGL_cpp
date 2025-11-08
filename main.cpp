#include "shader.hpp"
#include <GLFW/glfw3.h>
#include "model.hpp"
#include <iostream>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include "camera.hpp"
#include "stb_image.h"
#define pi 3.141592653589793f  
std::string read_file(std::string filename) {
    std::ifstream file(filename);
    std::string output,read;
    while (std::getline(file,read)) {
        output += read + "\n";
    }
    return output;
}
void resize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
int main() {
//window setup
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    glViewport(0, 0, 800, 800);
    glfwSetFramebufferSizeCallback(window, resize_callback);

//shader setup
    std::string vertex_source = read_file("vs.vert");
    const char* vertex_code = vertex_source.c_str();
    uint vs = compileShader(GL_VERTEX_SHADER, vertex_code);
    std::string fragment_source = read_file("fs.frag");
    const char* fragment_code = fragment_source.c_str();
    uint fs = compileShader(GL_FRAGMENT_SHADER, fragment_code);
    uint sp = setupProgram(vs, fs);
    std::string fragment_source2 = read_file("fs2.frag");
    const char* fragment_code2 = fragment_source2.c_str();
    uint fs2 = compileShader(GL_FRAGMENT_SHADER, fragment_code2);
    uint sp2 = setupProgram(vs, fs2);
//texture setup
    stbi_set_flip_vertically_on_load(true);
    /*glActiveTexture(GL_TEXTURE0);
    uint texture = loadTexture("brick.png");
    glActiveTexture(GL_TEXTURE1);
    uint specMap = loadTexture("specMap.png");*/

//mesh setup
/*
    float vertices[] = {
        //positions             //normals            //tex coords
         0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 0.0f, //front
         0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 0.0f, //back
        -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.5f, 1.0f,
         0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.5f, 0.0f,

         0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f, //top
         0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f, //bottom
        -0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 0.0f,

         0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f, //right
         0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f, //left
        -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
    };
    uint indices[] = {
         0,  1,  2,  0,  2,  3, //front
         4,  5,  6,  4,  6,  7, //back
         8,  9, 10,  8, 10, 11, //top
        12, 13, 14, 12, 14, 15, //bottom
        16, 17, 18, 16, 18, 19, //right
        20, 21, 22, 20, 22, 23  //left
    };
    int iCount = 36;
    uint vao, vbo, ebo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
*/
    glUseProgram(sp);
    /*Mesh cube = genCube(1, 1, 1);
    cube.material.diffuse = texture;
    cube.material.specular = specMap;
    cube.material.shininess = 32.0f;
    cube.setupMesh(sp);*/
    Model cube = loadModel("brick.obj");
    cube.meshes[0].setupMesh(sp);

//light setup
    glUseProgram(sp);
    uint lightTypeLoc = glGetUniformLocation(sp, "light.type");
    glUniform1i(lightTypeLoc, 3);
    uint lightPosLoc = glGetUniformLocation(sp, "light.position");
    glm::vec3 lightPos(0, 2, 0);
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
    uint lightDirLoc = glGetUniformLocation(sp, "light.direction");
    glUniform3f(lightDirLoc, 0, -1, 0);
    uint lightCutOffLoc = glGetUniformLocation(sp, "light.cutOff");
    glUniform1f(lightCutOffLoc, glm::cos(glm::radians(15.f)));
    uint lightOuterCutOffLoc = glGetUniformLocation(sp, "light.outerCutOff");
    glUniform1f(lightOuterCutOffLoc, glm::cos(glm::radians(20.f)));
    uint lightAmbientLoc = glGetUniformLocation(sp, "light.ambient");
    glUniform3f(lightAmbientLoc, 0.5f, 0.5f, 0.5f);
    uint lightDiffuseLoc = glGetUniformLocation(sp, "light.diffuse");
    glUniform3f(lightDiffuseLoc, 1.0f, 1.0f, 1.0f);
    uint lightSpecularLoc = glGetUniformLocation(sp, "light.specular");
    glUniform3f(lightSpecularLoc, 0.5f, 0.5f, 0.5f);
    uint lightConstantLoc = glGetUniformLocation(sp, "light.constant");
    glUniform1f(lightConstantLoc, 1.0f);
    uint lightLinearLoc = glGetUniformLocation(sp, "light.linear");
    glUniform1f(lightLinearLoc, 0.09f);
    uint lightQuadraticLoc = glGetUniformLocation(sp, "light.quadratic");
    glUniform1f(lightQuadraticLoc, 0.032f);
    uint viewPosLoc = glGetUniformLocation(sp, "viewPos");

//matrix setup
    uint modelLoc = glGetUniformLocation(sp, "model");
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(4, 0.1f, 4));
    glm::mat4 identity = glm::mat4(1.0f);
    uint viewLoc = glGetUniformLocation(sp, "view");
    Camera camera(glm::vec3(4, 1, 5), glm::vec3(0.0f, 1.0f, 0.0f), pi/4.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 proj = glm::perspective(camera.fov, 1.0f, 0.1f, 100.0f); 
    uint projLoc = glGetUniformLocation(sp, "proj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
    glUseProgram(sp2);
    glm::mat4 lightModel = glm::mat4(1.0f);
    uint modelLoc2 = glGetUniformLocation(sp2, "model");
    uint viewLoc2 = glGetUniformLocation(sp2, "view");
    uint projLoc2 = glGetUniformLocation(sp2, "proj");
    glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(proj));

//render loop
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glm::vec2 mousePos;
    bool click = false;
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            glm::vec3 dir(camera.front.x, 0.0f, camera.front.z);
            camera.position += glm::normalize(dir) * 0.05f;
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            lightPos.z -= 0.05f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            glm::vec3 dir(camera.front.x, 0.0f, camera.front.z);
            camera.position -= glm::normalize(dir) * 0.05f;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            lightPos.z += 0.05f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            glm::vec3 right = glm::normalize(glm::cross(camera.front, camera.up));
            camera.position -= right * 0.05f;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            lightPos.x -= 0.05f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            glm::vec3 right = glm::normalize(glm::cross(camera.front, camera.up));
            camera.position += right * 0.05f;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            lightPos.x += 0.05f;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.position.y += 0.05f;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            lightPos.y += 0.05f;
        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
            camera.position.y -= 0.05f;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            lightPos.y -= 0.05f;
        bool isClick = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        if (isClick && !click) {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            mousePos = glm::vec2(mouseX, mouseY);
            click = true;
        }
        if (!isClick) click = false;
        if (click) {
            glm::vec2 mouseDelta = mousePos;
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            mousePos = glm::vec2(mouseX, mouseY);
            mouseDelta -= mousePos;
            camera.yaw -= mouseDelta.x * 0.2f;
            camera.pitch += mouseDelta.y * 0.2f;
            if (camera.pitch > 85) camera.pitch = 85;
            if (camera.pitch < -85) camera.pitch = -85;
            camera.yaw = fmod(camera.yaw, 360.0f);
            camera.updateVectors();
        }
        glm::mat4 view = camera.getViewMatrix();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(sp);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(camera.position));
        //glBindVertexArray(vao);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //glDrawElements(GL_TRIANGLES, iCount, GL_UNSIGNED_INT, 0);
        cube.draw();
        glUseProgram(sp2);
        lightModel = glm::translate(identity, lightPos) * glm::scale(identity, glm::vec3(0.1f));
        glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(lightModel));
        glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
