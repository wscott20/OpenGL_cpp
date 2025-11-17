#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 tangent;
//layout(location = 4) in vec3 bitan;

out vec3 fragPos;
out vec3 fragNorm;
out vec2 fragTC;
out mat3 tbn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    vec4 worldPos = model * vec4(pos, 1.0);
    fragPos = worldPos.xyz;
    fragTC = texCoord;
    mat3 nmat = transpose(inverse(mat3(model)));
    fragNorm = normalize(nmat * normal);
    vec3 t = normalize(nmat * tangent.xyz);
    vec3 n = normalize(nmat * normal);
    //t = normalize(t - dot(t, n) * n);
    vec3 b = cross(n, t) * tangent.w;
    tbn = /* transpose */(mat3(t, b, n));
    gl_Position = proj * view * worldPos;
}