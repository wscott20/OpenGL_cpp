#include "OpenGL_utils.hpp"
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
    Mesh cube = genCube(bObj,10,.10,10);
    Model model{shader,{cube},Mat4::identity(),"model"};
    Mat4 rotateX = Mat4::identity(),
         rotateY = Mat4::identity(),
         translate = Mat4::identity();
    Camera cam{shader,"view","proj",Vec3{0,0,5},pi/4,Vec3{0,1,0},Vec3{1,0,0},Vec3{0,0,0}};
    shader.setUniform("numLights",1);
    Light test = Light::spot("lights[0]",shader,{0,3,0},{1,1,1},{0,-1,0},pi/4,1,.01,.01);
    //Mesh sphere = genSphere(bObj,2,10,20);
    //Model model{shader,{sphere},Mat4::identity(),"model"};
    model.setTexture("../img.jpeg");
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            translate = Mat4::viewTranslate({0,-.1,0}) * translate;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            translate = Mat4::viewTranslate({0,.1,0}) * translate;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            translate = Mat4::viewTranslate(Mat4Vec3(rotateY.transpose()*Mat4::translate({0,0,-.1}))) * translate;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            translate = Mat4::viewTranslate(Mat4Vec3(rotateY.transpose()*Mat4::translate({0,0,.1}))) * translate;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            translate = Mat4::viewTranslate(Mat4Vec3(rotateY.transpose()*Mat4::translate({.1,0,0}))) * translate;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            translate = Mat4::viewTranslate(Mat4Vec3(rotateY.transpose()*Mat4::translate({-.1,0,0}))) * translate;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);  //set the window to close
        if (glfwGetKey(window, GLFW_KEY_I))
            model.transform[1][3] += .1;
        if (glfwGetKey(window, GLFW_KEY_K))
            model.transform[1][3] -= .1;
        if (glfwGetKey(window, GLFW_KEY_UP)){
            auto rotateMat = Mat4::rotate(-1,X,Deg);;
            rotateX *= rotateMat;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN)){
            auto rotateMat = Mat4::rotate(1,X,Deg);;
            rotateX *= rotateMat;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT)){
            auto rotateMat = Mat4::rotate(1,Y,Deg);
            rotateY *= rotateMat;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT)){
            auto rotateMat = Mat4::rotate(-1,Y,Deg);
            rotateY *= rotateMat;
        }
        cam.view = rotateX * rotateY * translate;
        // test.update();
        cam.update(); 
        glClearColor(.3,.3,.3,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        model.draw(bObj);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    shader.del();
    glfwTerminate();
    return 0;
}