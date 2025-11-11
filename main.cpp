#include "shader.hpp"
#include <GLFW/glfw3.h>
#include "model.hpp"
#include <iostream>
#include "camera.hpp"
#include "stb_image.h"
#define pi 3.141592653589793f  
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
#ifdef __APPLE__
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
    GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSize(window,800,800);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    glViewport(0, 0, 800, 800);
    glfwSetFramebufferSizeCallback(window, resize_callback);

//shader setup
    Shader  shader("vs.vert", "fs.frag"),
            lightShader("vs.vert", "fs2.frag"),
            cubemapShader("cubemap.vert", "cubemap.frag");
    stbi_set_flip_vertically_on_load(true);

//mesh setup
    shader.use();
    Model cube = loadModel("brick.obj");
    cube.meshes[0].setupMesh();
    cube.meshes[0].setupMaterial(shader);

//light setup
    shader.setInt("light.type", 3);
    glm::vec3 lightPos(0, 2, 0);
    int lightPosLoc = shader.getLoc("light.position");
    shader.setVec3(lightPosLoc, lightPos);
    int lightDirLoc = shader.getLoc("light.direction");
    glUniform3f(lightDirLoc, 0, -1, 0);
    int lightCutOffLoc = shader.getLoc("light.cutOff");
    shader.setFloat(lightCutOffLoc, glm::cos(glm::radians(15.f)));
    int lightOuterCutOffLoc = shader.getLoc("light.outerCutOff");
    shader.setFloat(lightOuterCutOffLoc, glm::cos(glm::radians(20.f)));
    int lightAmbientLoc = shader.getLoc("light.ambient");
    shader.setVec3(lightAmbientLoc, 0.5f, 0.5f, 0.5f);
    int lightDiffuseLoc = shader.getLoc("light.diffuse");
    shader.setVec3(lightDiffuseLoc, 1.0f, 1.0f, 1.0f);
    int lightSpecularLoc = shader.getLoc("light.specular");
    shader.setVec3(lightSpecularLoc, 0.5f, 0.5f, 0.5f);
    int lightConstantLoc = shader.getLoc("light.constant");
    shader.setFloat(lightConstantLoc, 1.0f);
    int lightLinearLoc = shader.getLoc("light.linear");
    shader.setFloat(lightLinearLoc, 0.09f);
    int lightQuadraticLoc = shader.getLoc("light.quadratic");
    shader.setFloat(lightQuadraticLoc, 0.032f);
    int viewPosLoc = shader.getLoc("viewPos");

//matrix setup
    uint modelLoc = shader.getLoc("model");
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
    glm::mat4 identity = glm::mat4(1.0f);
    uint viewLoc = shader.getLoc("view");
    Camera camera(glm::vec3(4, 1, 5), glm::vec3(0.0f, 1.0f, 0.0f), pi/4.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 proj = glm::perspective(camera.fov, 1.0f, 0.1f, 100.0f); 
    shader.setMat4("proj", proj);
    lightShader.use();
    glm::mat4 lightModel = glm::mat4(1.0f);
    uint modelLoc2 = lightShader.getLoc("model");
    uint viewLoc2 = lightShader.getLoc("view");
    lightShader.setMat4("proj", proj);

//cubemap
    uint cubemap;
    glGenTextures(1, &cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    std::string filenames[] = {
        "front.png",
        "back.png",
        "right.png",
        "left.png",
        "top.png",
        "bottom.png",
    };
    int w, h, nChan;
    unsigned char* data;
    for (int i = 0; i < 6; i++) {
        data = stbi_load(filenames[i].c_str(), &w, &h, &nChan, 0);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
            0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

//skybox
    float skyboxVerts[] = {
         1, -1, -1,
         1,  1, -1,
        -1,  1, -1,
        -1, -1, -1,

        -1, -1,  1,
        -1,  1,  1,
         1,  1,  1,
         1, -1,  1,

         1, -1, -1,
         1,  1, -1,
         1,  1,  1,
         1, -1,  1,

        -1, -1,  1,
        -1,  1,  1,
        -1,  1, -1,
        -1, -1, -1,

        -1,  1, -1,
        -1,  1,  1,
         1,  1,  1,
         1,  1, -1,

        -1, -1,  1,
        -1, -1, -1,
         1, -1, -1,
         1, -1,  1,
    };
    uint skyboxIndices[] = {
         0,  1,  2,  0,  2,  3,
         4,  5,  6,  4,  6,  7,
         8,  9, 10,  8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23,
    };
    uint skyboxVbo, skyboxVao, skyboxEbo;
    glGenVertexArrays(1, &skyboxVao);
    glBindVertexArray(skyboxVao);
    glGenBuffers(1, &skyboxVbo);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVerts), skyboxVerts, GL_STATIC_DRAW);
    glGenBuffers(1, &skyboxEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), skyboxIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    cubemapShader.use();
    cubemapShader.setMat4("proj", proj);
    cubemapShader.setInt("cubemap", 0);
    int cubemapViewLoc = cubemapShader.getLoc("view");
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cubemapShader.use();
        glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));  
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        cubemapShader.setMat4(cubemapViewLoc, view);
        glBindVertexArray(skyboxVao);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        
        shader.use();
        view = camera.getViewMatrix();
        shader.setMat4(viewLoc, view);
        shader.setVec3(lightPosLoc, lightPos);
        shader.setVec3(viewPosLoc, camera.position);
        shader.setMat4(modelLoc, model);
        glBindTexture(GL_TEXTURE_2D, cube.meshes[0].material.diffuse);
        cube.draw();

        lightShader.use();
        lightModel = glm::translate(identity, lightPos) * glm::scale(identity, glm::vec3(0.1f));
        lightShader.setMat4(modelLoc2, lightModel);
        lightShader.setMat4(viewLoc2, view);
        cube.draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
