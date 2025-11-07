#pragma once
using uint = unsigned int;
enum MatLocs {
    DIFFUSE_LOC,
    SPECULAR_LOC,
    SHININESS_LOC,
    MATLOC_COUNT
};
struct Material {
    uint diffuse;
    uint specular;
    float shininess;
    uint locs[MATLOC_COUNT];
};