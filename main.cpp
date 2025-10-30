#include "OpenGL_input.hpp"
#include "Camera.hpp"
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
    Mesh heightmap{121,600};
    std::vector<Vec3> verts={},colors={},norms={};
    std::vector<Vec2> tc={};
    std::vector<uint> indices={};
    for (int x=0;x<=10;x++) {
        for (int z=0;z<=10;z++) {
            uint inds[]={
                z*11+x+1,
                (z+1)*11+x+1,
                (z+1)*11+x,
                z*11+x+1,
                (z+1)*11+x,
                z*11+x,
            };
            if(x<10&&z<10)
                for(auto ind:inds)
                    indices.push_back(ind);
            Vec3 vert={x*10-50,0,z*10-50};
            verts.push_back(vert);
            Vec3 c={1,1,1},n={0,1,0};
            colors.push_back(c);
            norms.push_back(n);
            Vec2 texc={0,0};
            tc.push_back(texc);
        }
    }
    heightmap.setPositions(verts);
    heightmap.setIndices(indices);
    heightmap.setTexCoords(tc);
    heightmap.setColors(colors);
    heightmap.setNorms(norms);
    Model model{shader,{heightmap},Mat4::identity(),"model"};
    Mat4 rotateX = Mat4::identity(),
         rotateY = Mat4::identity(),
         translate = Mat4::identity();
    Camera cam{shader,"view","proj",Vec3{0,0,5},pi/4,Vec3{0,1,0},Vec3{1,0,0},Vec3{0,0,0}};
    //model.setTexture("../img.jpeg");
    while (!glfwWindowShouldClose(window)) {
        if (get_key(window,"q"))
            translate = Mat4::viewTranslate({0,-.1,0}) * translate;
        if (get_key(window,"e"))
            translate = Mat4::viewTranslate({0,.1,0}) * translate;
        if (get_key(window,"w"))
            translate = Mat4::viewTranslate(Mat4Vec3(rotateY.transpose()*Mat4::translate({0,0,-.1}))) * translate;
        if (get_key(window,"s"))
            translate = Mat4::viewTranslate(Mat4Vec3(rotateY.transpose()*Mat4::translate({0,0,.1}))) * translate;
        if (get_key(window,"d"))
            translate = Mat4::viewTranslate(Mat4Vec3(rotateY.transpose()*Mat4::translate({.1,0,0}))) * translate;
        if (get_key(window,"a"))
            translate = Mat4::viewTranslate(Mat4Vec3(rotateY.transpose()*Mat4::translate({-.1,0,0}))) * translate;
        if (get_key(window,"esc"))
            glfwSetWindowShouldClose(window, true);  //set the window to close
        if (get_key(window, "up")){
            auto rotateMat = Mat4::rotate(-1,X,Deg);;
            rotateX *= rotateMat;
        }
        if (get_key(window, "down")){
            auto rotateMat = Mat4::rotate(1,X,Deg);;
            rotateX *= rotateMat;
        }
        if (get_key(window, "right")){
            auto rotateMat = Mat4::rotate(1,Y,Deg);
            rotateY *= rotateMat;
        }
        if (get_key(window, "left")){
            auto rotateMat = Mat4::rotate(-1,Y,Deg);
            rotateY *= rotateMat;
        }
        cam.view = rotateX * rotateY * translate;
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