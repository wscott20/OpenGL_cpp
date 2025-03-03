#ifndef OpenGL_math_hpp
#define OpenGL_math_hpp
#include <array>
#include <cmath>
#include <variant>
#define pi 3.14159f //approximately pi
enum Axis {X,Y,Z};
enum Angle {Rad,Deg};
#define sq(n) (n*n)
struct Vec2 {
    float x, y;
    Vec2(): x{0}, y{0} {}
    Vec2(float x_, float y_): x{x_}, y{y_} {} //used _ to avoid ambiguity
    Vec2(const Vec2& other): x{other.x}, y{other.y} {}
    inline float magnitude() {return sqrt(sq(x) + sq(y));}
    Vec2 normalize() {
        float mag = magnitude();
        return Vec2{x/mag, y/mag};
    }
    static float dot(Vec2 a, Vec2 b) {
        return (a.x * b.x) + (a.y * b.y);
    }
    static float cross(Vec2 a, Vec2 b) {
        return (a.x * b.y) - (a.y * b.x);
    }
    static Vec2 dir(Vec2 a, Vec2 b) {
        return (b-a).normalize();
    }
    Vec2 operator-(Vec2 other) {
        return Vec2{x - other.x, y - other.y};
    }
    Vec2 operator+(Vec2 other) {
        return Vec2{x + other.x, y + other.y};
    }
    Vec2 operator-() {return (*this) * -1;}
    Vec2 operator*(Vec2 other) {
        return Vec2{x*other.x, y*other.y};
    }
    Vec2 operator*(std::variant<int,float,double> n) {
        Vec2 v;
        if(auto ptr = std::get_if<float>(&n)) v.x=v.y=*ptr;
        else if(auto ptr = std::get_if<int>(&n)) v.x=v.y=(float)(*ptr);
        else if(auto ptr = std::get_if<double>(&n)) v.x=v.y=(float)(*ptr);
        return (*this) * v;
    }
    float* data() {
        static float vec[] = {x,y};
        return vec;
    }
};
struct Vec3 {
    float x, y, z;
    Vec3(): x{0}, y{0}, z{0} {}
    Vec3(float x_, float y_, float z_): x{x_}, y{y_}, z{z_} {} //used _ to avoid ambiguity
    Vec3(const Vec3& other): x{other.x}, y{other.y}, z{other.z} {}
    inline float magnitude() {return sqrt(sq(x) + sq(y) + sq(z));}
    Vec3 normalize() {
        float mag = magnitude();
        return Vec3{x/mag, y/mag, z/mag};
    }
    static float dot(Vec3 a, Vec3 b) {
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    }
    static Vec3 cross(Vec3 a, Vec3 b) {
        return Vec3{
            (a.y * b.z) - (a.z * b.y),
            (a.z * b.x) - (a.x * b.z),
            (a.x * b.y) - (a.y * b.x)
        };
    }
    static Vec3 dir(Vec3 a, Vec3 b) {
        return (b-a).normalize();
    }
    Vec3 operator-(Vec3 other) {
        return Vec3{x - other.x, y - other.y, z - other.z};
    }
    Vec3 operator+(Vec3 other) {
        return Vec3{x + other.x, y + other.y, z + other.z};
    }
    Vec3 operator-() {return (*this) * -1;}
    Vec3 operator*(Vec3 other) {
        return Vec3{x*other.x, y*other.y, z*other.z};
    }
    Vec3 operator*(std::variant<int,float,double> n) {
        Vec3 v;
        if(auto ptr = std::get_if<float>(&n)) v.x=v.y=v.z=*ptr;
        else if(auto ptr = std::get_if<int>(&n)) v.x=v.y=v.z=(float)(*ptr);
        else if(auto ptr = std::get_if<double>(&n)) v.x=v.y=v.z=(float)(*ptr);
        return (*this) * v;
    }
    float* data() {
        static float vec[] = {x,y,z};
        return vec;
    }
};
Vec2 operator*=(Vec2& a, std::variant<int,float,double> b) {
    a = a * b;
    return a;
}
Vec3 operator*=(Vec3& a, std::variant<int,float,double> b) {
    a = a * b;
    return a;
}
Vec2 operator*=(Vec2& a, Vec2 b) {
    a = a * b;
    return a;
}
Vec3 operator*=(Vec3& a, Vec3 b) {
    a = a * b;
    return a;
}
Vec2 operator+=(Vec2& a, Vec2 b) {
    a = a + b;
    return a;
}
Vec3 operator+=(Vec3& a, Vec3 b) {
    a = a + b;
    return a;
}
Vec2 operator-=(Vec2& a, Vec2 b) {
    a = a - b;
    return a;
}
Vec3 operator-=(Vec3& a, Vec3 b) {
    a = a - b;
    return a;
}
using float4x4 = std::array<std::array<float,4>,4>;
class Mat4 {
    float4x4 data;
    public:
    Mat4() {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                data[i][j] = 0;
    }
    Mat4(float4x4 mat): data{mat} {} 
    Mat4(const Mat4& other) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                data[i][j] = other[i][j];
    }
    std::array<float,4>& operator[](int n) {
        return data[n];
    }
    const std::array<float,4>& operator[](int n) const {
        return data[n];
    }
    Mat4 operator*(Mat4 other) {
        Mat4 mat;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                for (int k = 0; k < 4; k++)
                    mat[i][j] += data[i][k] * other[k][j];
        return mat;
    }
    static Mat4 identity() {
        Mat4 mat; //initializes values to 0
        for (int i = 0; i < 4; i++)
            mat[i][i] = 1;
        return mat;
    }
    static Mat4 perspective(float fov,float aspect,float z_near,float z_far) {
        float4x4 mat{{
            {1/(aspect * tan(fov/2)), 0, 0, 0},
            {0, 1/tan(fov/2), 0, 0},
            {0, 0, (z_far + z_near)/(z_near - z_far), (2 * z_far * z_near)/(z_near - z_far)},
            {0, 0, -1, 0}
        }};
        return Mat4{mat};
    }
    static Mat4 rotate(float t, Axis axis, Angle rad_or_deg) { //t stands for theta (or angle)
        if (rad_or_deg == Deg) t *= pi/180;
        float4x4 mat;
        switch(axis) {
            case X: {
                mat = {{
                    {1, 0,      0,       0},
                    {0, cos(t), -sin(t), 0},
                    {0, sin(t), cos(t),  0},
                    {0, 0,      0,       1}
                }};
                break;
            }
            case Y: {
                mat = {{
                    {cos(t),  0, sin(t), 0},
                    {0,       1, 0,      0},
                    {-sin(t), 0, cos(t), 0},
                    {0,       0, 0,      1}
                }};
                break;
            }
            case Z: {
                mat = {{
                    {cos(t), -sin(t), 0, 0},
                    {sin(t), cos(t),  0, 0},
                    {0,      0,       1, 0},
                    {0,      0,       0, 1}
                }};
                break;
            }
        }
        return Mat4{mat};
    }
    Mat4 transpose() {
        Mat4 mat;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                mat[i][j] = data[j][i];
        return mat;
    }
    static Mat4 translate(float x,float y,float z) {
        Mat4 mat = identity();
        mat[0][3] = x;
        mat[1][3] = y;
        mat[2][3] = z;
        return mat;
    }
    static Mat4 translate(Vec3 v) {return translate(v.x,v.y,v.z);}
    static Mat4 viewTranslate(float x,float y,float z) { //translation for view matrix
        return translate({-x,-y,-z});
    }
    static Mat4 viewTranslate(Vec3 v) {return viewTranslate(v.x,v.y,v.z);}
    static Mat4 scale(float x,float y,float z) {
        auto mat = identity();
        mat[0][0] = x;
        mat[1][1] = y;
        mat[2][2] = z;
        return mat;
    }
    static Mat4 scale(Vec3 v) {return scale(v.x,v.y,v.z);}
    static Mat4 lookAt(Vec3 up, Vec3 right, Vec3 dir, Vec3 pos) {
        Mat4 mat1{{{
            {right.x, right.y, right.z, 0},
            {up.x,    up.y,    up.z,    0},
            {-dir.x,  -dir.y,  -dir.z,  0},
            {0,       0,       0,       1}
        }}}, mat2 = viewTranslate(pos);
        return mat1 * mat2;
    }
    float* glPtr() {
        static float stuff[16];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                stuff[i * 4 + j] = data[j][i];
        return stuff;
    }
};
Mat4 operator*=(Mat4& m1,Mat4& m2) {
    m1 = m2 * m1; //matrices multiply  reverse order
    return m1;
}
Vec3 Mat4Vec3(Mat4 mat) { //get vec3 position from mat4
    return {mat[0][3],mat[1][3],mat[2][3]};
}
#endif