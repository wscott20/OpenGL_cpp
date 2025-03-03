#version 330 core
#define Directional 0
#define Point 1
#define Ambient 2
#define Spot 3

in vec3 fragPos;
in vec3 fragColor;
in vec3 norm;
in vec2 texCoord;

struct Light {
    int type;
    vec3 position;
    vec3 color;
    vec3 direction;
    float intensity;
    float cutoff;
    float constant;
    float linear;
    float quadratic;
};

uniform sampler2D tex;
uniform int numLights;
uniform Light lights[10];
uniform bool useTex;

out vec4 color;
void main() {
    vec3 lightColor = vec3(0,0,0);
    for (int i = 0; i < numLights; i++) {
        Light light = lights[i];
        vec3 dir;
        vec3 diff;
        float dist;
        float att;
        switch(light.type) {
            case Ambient:
                lightColor += light.color * light.intensity;
                break;
            case Directional:
                dir = -normalize(light.direction);
                diff = light.color * max(dot(norm,dir),0);// * light.intensity;
                lightColor += diff;
                break;
            case Point:
                dir = fragPos - light.position;
                dist = length(dir);
                att = 1 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
                diff = light.color * max(dot(-norm, normalize(dir)), 0) * att;
                lightColor += diff;
                break;
            case Spot:
                dir = fragPos - light.position;
                dist = length(dir);
                //att = 1 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
                float angle = dot(normalize(dir),normalize(light.direction));
                if (acos(angle) < light.cutoff) {
                    diff = light.color * max(dot(-norm, normalize(dir)), 0);// * att;
                    lightColor += diff;
                }
                break;
            default:break;
        }
    }
    color = vec4(fragColor,1) * vec4(lightColor,1);
    if (useTex) color *= texture(tex,texCoord); 
}