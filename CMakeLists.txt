cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(cs200-assignment-6 VERSION 1)

set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(GLEW REQUIRED)

add_compile_options (-fdiagnostics-color=always)
add_compile_options(-Wextra -Wall -Wpedantic -std=c++11)

# Assignment 1
add_library(Affine Affine.cpp)
target_link_libraries(Affine PRIVATE glm::glm)

# Assignment 2 My Mesh (Not included)
# add_library(MyMesh MyMesh.cpp)
# target_link_libraries(MyMesh PRIVATE glm::glm)

# Assignment 2 Square Mesh
add_library(SquareMesh SquareMesh.cpp)
target_link_libraries(SquareMesh PRIVATE glm::glm)

# Assignment 3
add_library(SolidRender SolidRender.cpp)
target_link_libraries(SolidRender PRIVATE glm::glm)

# Assignment 4
add_library(Camera Camera.cpp)
target_link_libraries(Camera PRIVATE glm::glm)

# Assignment 5
add_library(TextureLoad Texture.cpp)
target_link_libraries(TextureLoad PRIVATE glm::glm)

# Assignment 6
add_library(TextureRender TextureRender.cpp)
target_link_libraries(TextureRender PRIVATE glm::glm)

# DPTexturedMesh
add_library(DPMesh DPTexturedMesh.cpp)
target_link_libraries(DPMesh PRIVATE glm::glm)

# Optional Assignment 1
add_library(MyTiledMesh MyTiledMesh.cpp)
target_link_libraries(MyTiledMesh PRIVATE glm::glm)
add_library(MyTexturedMesh MyTexturedMesh.cpp)
target_link_libraries(MyTexturedMesh PRIVATE glm::glm)

# Linking all to test driver 1
add_executable(TiledTest MyTiledMeshTest.cpp)
target_link_libraries(TiledTest PRIVATE MyTexturedMesh MyTiledMesh DPMesh TextureRender TextureLoad Camera SquareMesh SolidRender Affine OpenGL glm::glm SDL2::SDL2 ${GLEW_LIBRARIES})
include_directories(TiledTest ${GLEW_INCLUDE_DIRS})

# Linking all to test driver 2
add_executable(TexturedTest MyTexturedMeshTest.cpp)
target_link_libraries(TexturedTest PRIVATE MyTexturedMesh MyTiledMesh  DPMesh TextureRender TextureLoad Camera SquareMesh SolidRender Affine OpenGL glm::glm SDL2::SDL2 ${GLEW_LIBRARIES})
include_directories(TexturedTest ${GLEW_INCLUDE_DIRS})

# Custom tests
add_executable(Custom Custom.cpp)

# vim:shiftwidth=2:
