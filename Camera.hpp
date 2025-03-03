#ifndef camera_hpp
#define camera_hpp
#include "OpenGL_math.hpp"
class Camera {
    cstr viewName,projName;
    float fov;
    Shader shader;
    public:
    Mat4 view, proj;
    Vec3 up, right, position, target;
    Camera(Shader s, cstr vn, cstr pn, Vec3 pos, float fovy, Vec3 upV, Vec3 rightV, Vec3 tg): shader{s}, position{pos}, viewName{vn}, projName{pn}, fov{fovy}, up{upV}, right{rightV}, target{tg} {
        view = Mat4::lookAt(up,right,(target-pos).normalize(),pos);
        proj = Mat4::perspective(fov,1,.1,100);
        shader.setUniform(projName,proj);
    }
    void update() {
        shader.setUniform(viewName,view);
    }
    void rotate(Mat4 rotation) {
        view *= rotation;
    }
    void rotate(float angle, Axis axis, Angle angle_type) {
        Mat4 rotation = Mat4::rotate(angle,axis,angle_type);
        view *= rotation;
    }
};
#endif