#include "OpenGL_input.hpp"
#include "Camera.hpp"
#include "Light.hpp"
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(800,800,"OpenGL",NULL,NULL);
    if (window == NULL) {
        std::cout << "failed to make GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    BufferObj bObj;
    bObj.bind();
    Shader shader{"../vertShader.vert","../fragShader.frag"};
    //had to use ../ since running from build directory instead of root
    if (shader.status == -1) {
        //error already prints in constructor
        glfwTerminate();
        return -1;
    }
    shader.use();
    shader.setUniform("useTex",0);
    Mesh cube = genCube(bObj,1,1,1,{1,0,0,.5});
    Camera cam(shader,"view","proj",vec3{0,1,5},pi/4,vec3{0,1,0},vec3{0,0,0},1);
    shader.setUniform("numLights",1);
    Light ambient = Light::ambient("lights[0]",shader,{1,1,1},1);
    mat4 model = identity();
    shader.setUniform("model",model);
    glClearColor(.3,.3,.3,1);
    while (!glfwWindowShouldClose(window)) {
        if (get_key(window,"q"))
            cam.position.y += .1f;
        if (get_key(window,"e"))
            cam.position.y -= .1f;
        if (get_key(window,"w")) {
            vec3 forward = glm::normalize(vec3{cam.front.x,0,cam.front.z});
            cam.position += forward * .1f;
        }
        if (get_key(window,"s")) {
            vec3 backward = glm::normalize(vec3{cam.front.x,0,cam.front.z});
            cam.position -= backward * .1f;
        }
        if (get_key(window,"d")) {
            vec3 right = glm::normalize(glm::cross(cam.front, cam.up));
            cam.position += right * .1f;
        }
        if (get_key(window,"a")) {
            vec3 right = glm::normalize(glm::cross(cam.front, cam.up));
            cam.position -= right * .1f;
        }
        if (get_key(window, "up")){
            cam.pitch = std::min(cam.pitch + 0.02f, pi/2 - 0.01f);
        }
        if (get_key(window, "down")){
            cam.pitch = std::max(cam.pitch - 0.02f, -pi/2 + 0.01f);
        }
        if (get_key(window, "right")){
            cam.yaw += 0.02f;
            cam.yaw = fmod(cam.yaw, 2 * pi);
        }
        if (get_key(window, "left")){
            cam.yaw -= 0.02f;
            cam.yaw = fmod(cam.yaw, 2 * pi);
        }
        if (get_key(window,"esc"))
            glfwSetWindowShouldClose(window, true);  //set the window to close
        cam.update();
        shader.setUniform("model",model);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, cube.iCount, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    shader.del();
    glfwTerminate();
    return 0;
}