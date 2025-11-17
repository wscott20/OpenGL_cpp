#version 330 core
#define Ambient 0
#define Directional 1
#define Point 2
#define Spot 3
#define MAX_LIGHTS 16

in vec3 fragPos;
in vec3 fragNorm;
in vec2 fragTC;
in mat3 tbn;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};

uniform Material material;
uniform Light lights[MAX_LIGHTS];
uniform sampler2D normMap;
uniform int nLights;
uniform vec3 viewPos;
out vec4 color;

vec3 calcSpecular(vec3 norm, vec3 lightSpec, vec3 lightDir, vec3 viewDir) {
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specMap = texture(material.specular, fragTC).rgb;
    return specMap * spec * lightSpec;
}
void main() {
    vec3 nmap = normalize(texture(normMap, fragTC).rgb * 2.f - 1.f);
    vec3 norm = normalize(tbn*nmap);
    vec3 albedo = texture(material.diffuse, fragTC).rgb;
    vec3 finalColor = vec3(0);
    for (int i = 0; i < nLights; i++) {
        Light light = lights[i];
        vec3 ambient = light.ambient * albedo;
        finalColor += ambient;
        if (light.type == Ambient) continue;
        vec3 lightDir =
            light.type == Directional
            ? normalize(-light.direction)
            : normalize(light.position - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = (albedo * diff) * light.diffuse;
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 specular = calcSpecular(norm, light.specular, lightDir, viewDir);
        if (light.type == Spot || light.type == Point) {
            float dist = length(light.position - fragPos);
            float att = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
            diffuse *= att;
            specular *= att;
            if (light.type == Spot) {
                float theta = dot(lightDir, normalize(-light.direction));
                if (theta <= light.outerCutOff) {
                    diffuse = vec3(0);
                    specular = vec3(0);
                } else if (theta < light.cutOff) {
                    float epsilon = light.cutOff - light.outerCutOff;
                    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
                    diffuse *= intensity;
                    specular *= intensity;
                }
            }
        }
        finalColor += diffuse;// + specular;
    }
    color = vec4(finalColor, 1.0);
    // color = vec4(norm/2+.5,1);
}