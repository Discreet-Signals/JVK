/*
  ==============================================================================

    Mesh.h
    Created: 13 Oct 2023 6:15:14pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once
#include <glm/glm.hpp>

namespace jdk::Objects::Mesh
{

namespace Vertices
{

struct Vertex
{
    glm::vec3 position;
};

struct ColoredVertex
{
    glm::vec3 position;
    glm::vec4 color;
};

struct DirectionalVertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

struct DirectionalColoredVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec4 color;
};

} // Vertices

template <typename V>
struct Mesh
{
    glm::mat4x4 transform;
    std::vector<V> vertices;
    std::vector<int> indices;
};

} // jdk::Objects::Mesh
