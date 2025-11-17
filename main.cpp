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
            lightShader("vs.vert", "fs2.frag")/*,
            hdrShader("hdr.vert", "hdr.frag")*/;
    stbi_set_flip_vertically_on_load(true);

//mesh setup
    shader.use();
    Mesh cube = genCube(1,1,1);
    cube.material.diffuse = loadTexture("brick.png");
    cube.material.specular = loadTexture("specMap2.png");
    cube.setupMaterial(shader);
    shader.setInt("normMap",2);
    glActiveTexture(GL_TEXTURE2);
    uint normMap; //= loadTexture("brickNorm.png");
    glGenTextures(1, &normMap);
    int width, height, nrComponents;
    unsigned char *data = stbi_load("brickNorm.png", &width, &height, &nrComponents, 3);
    if (data) {
        glBindTexture(GL_TEXTURE_2D, normMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else std::cerr << "Normal map failed to load" << std::endl;
    stbi_image_free(data);
    /*float quadVerts[] = {
         1, -1, 0,  1, 0,
         1,  1, 0,  1, 1,
        -1,  1, 0,  0, 1,
        -1, -1, 0,  0, 0,
    };
    uint quadIndices[] = {
        0, 1, 2, 0, 2, 3,
    };
    uint quadVao, quadVbo, quadEbo;
    glGenVertexArrays(1, &quadVao);
    glBindVertexArray(quadVao);
    glGenBuffers(1, &quadVbo);
    glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);
    glGenBuffers(1, &quadEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));*/

//light setup
    shader.setInt("nLights", 1);
    shader.setInt("lights[0].type", 1);
    glm::vec3 lightPos(0, 2, 0);
    //int lightPosLoc = shader.getLoc("lights[0].position");
    //shader.setVec3(lightPosLoc, lightPos);
    int dirLoc = shader.getLoc("lights[0].direction");
    shader.setVec3(dirLoc, 0, -1, 0);
    shader.setVec3("lights[0].ambient", 0.1f, 0.1f, 0.1f);
    shader.setVec3("lights[0].diffuse", 1.0f, 1.0f, 1.0f);
    shader.setVec3("lights[0].specular", 0.5f, 0.5f, 0.5f);
    //shader.setFloat("lights[0].cutOff", glm::cos(glm::radians(15.f)));
    //shader.setFloat("lights[0].outerCutOff", glm::cos(glm::radians(20.f)));
    //shader.setFloat("lights[0].constant", 1.0f);
    //shader.setFloat("lights[0].linear", 0.09f);
    //shader.setFloat("lights[0].quadratic", 0.032f);
    int viewPosLoc = shader.getLoc("viewPos");

//hdr setup
    /*uint hdrfbo;
    glGenFramebuffers(1, &hdrfbo);
    uint colorbuffer;
    glGenTextures(1, &colorbuffer);
    glBindTexture(GL_TEXTURE_2D, colorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 800, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glBindTexture(GL_TEXTURE_2D, 0);
    uint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 800);  
    glBindFramebuffer(GL_FRAMEBUFFER, hdrfbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorbuffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    hdrShader.use();
    int hdrLoc = hdrShader.getLoc("hdr");
    bool useHdr = false;
    hdrShader.setInt("hdrBuffer", 0);
    hdrShader.setFloat("exposure", .1f);
    hdrShader.setInt("gammaCorrection", false);*/

//matrix setup
    //shader.use();
    uint modelLoc = shader.getLoc("model");
    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 model = identity;//glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
    uint viewLoc = shader.getLoc("view");
    Camera camera(glm::vec3(4, 1, 5), glm::vec3(0.0f, 1.0f, 0.0f), pi/4.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 proj = glm::perspective(camera.fov, 1.0f, 0.1f, 100.0f); 
    shader.setMat4("proj", proj);
    lightShader.use();
    glm::mat4 lightModel = glm::mat4(1.0f);
    uint modelLoc2 = lightShader.getLoc("model");
    uint viewLoc2 = lightShader.getLoc("view");
    lightShader.setMat4("proj", proj);

//render loop
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glm::vec2 mousePos;
    bool click = false;
    while (!glfwWindowShouldClose(window)) {
    //input
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
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            lightPos.y += 0.05f;
        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
            camera.position.y -= 0.05f;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
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
    //render
        //glBindFramebuffer(GL_FRAMEBUFFER, hdrfbo);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glm::mat4 view = camera.getViewMatrix();
        shader.setVec3(dirLoc,glm::normalize(-lightPos));
        shader.setMat4(viewLoc, view);
        //shader.setVec3(lightPosLoc, lightPos);
        shader.setVec3(viewPosLoc, camera.position);
        shader.setMat4(modelLoc, model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cube.material.diffuse);
        cube.draw();

        lightShader.use();
        lightModel = glm::translate(identity, lightPos) * glm::scale(identity, glm::vec3(0.1f));
        lightShader.setMat4(modelLoc2, lightModel);
        lightShader.setMat4(viewLoc2, view);
        cube.draw();
        /*glBindFramebuffer(GL_FRAMEBUFFER, 0);
        hdrShader.use();
        glClearColor(0.1f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        useHdr = glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS;
        hdrShader.setInt(hdrLoc, useHdr ? 1 : 0);
        glBindVertexArray(quadVao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorbuffer);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
