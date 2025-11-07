cmake_minimum_required(VERSION 3.10)
project(learnopengl)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)
add_subdirectory(glad)
add_executable(learnopengl main.cpp)
target_include_directories(learnopengl PRIVATE ${CMAKE_SOURCE_DIR}/include)
set(GLAD_SOURCES glad/glad.c)
target_sources(learnopengl PRIVATE
    ${GLAD_SOURCES}
    shader.cpp
    camera.cpp
    mesh.cpp
    model.cpp
)
target_link_libraries(learnopengl PRIVATE
    OpenGL::GL
    glfw
    glad
    assimp::assimp
    X11
    pthread
    Xrandr
    Xi
    dl
)