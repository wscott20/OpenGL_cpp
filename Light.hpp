#pragma once
#include "glm_header.hpp"
enum LightType {
    Directional=0,
    Point=1,
    Ambient=2,
    Spot=3
};
class Light {
    string locName;
    Shader shader;
    public:
    vec3 position,direction,color;
    LightType type;
    float intensity,constant,linear,quadratic,cutoff;
    Light(string name,Shader s): locName{name},shader{s} {}
    void update() {
        //will add separate functions later to update specific attributes
        shader.setUniform((locName + ".type").c_str(),type);
        shader.setUniform((locName + ".position").c_str(),position);
        shader.setUniform((locName + ".direction").c_str(),direction);
        shader.setUniform((locName + ".color").c_str(),color);
        shader.setUniform((locName + ".intensity").c_str(),intensity);
        shader.setUniform((locName + ".constant").c_str(),constant);
        shader.setUniform((locName + ".linear").c_str(),linear);
        shader.setUniform((locName + ".quadratic").c_str(),quadratic);
        shader.setUniform((locName + ".cutoff").c_str(),cutoff);
    }
    static Light ambient(cstr name,Shader s,vec3 color,float intensity) {
        Light light{name,s};
        light.color = color;
        light.intensity = intensity;
        light.type = Ambient;
        light.update();
        return light;
    }
    static Light directional(cstr name,Shader s,vec3 col,vec3 dir) {
        Light light{name,s};
        light.color = col;
        light.direction = dir;
        light.type = Directional;
        light.update();
        return light;
    }
    static Light point(cstr name,Shader s,vec3 pos,vec3 col,float cons,float lin,float quad) {
        Light light{name,s};
        light.position = pos;
        light.color = col;
        light.constant = cons;
        light.linear = lin;
        light.quadratic = quad;
        light.type = Point;
        light.update();
        return light;
    }
    static Light spot(cstr name,Shader s,vec3 pos,vec3 col,vec3 dir,float cutOff,float cons,float lin,float quad) {
        Light light{name,s};
        light.position = pos;
        light.color = col;
        light.direction = dir;
        light.constant = cons;
        light.linear = lin;
        light.quadratic = quad;
        light.cutoff = cutOff;
        light.type = Spot;
        light.update();
        return light;
    }
};