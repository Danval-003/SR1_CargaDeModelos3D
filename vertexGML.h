#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <array>

struct Face {
    std::vector<int> vertexIndices;

    Face(): vertexIndices(){};
    Face(std::vector<int> temp_vertex): vertexIndices(temp_vertex){};
};

// En vertexGML.h, cambia la definición de la estructura VertexGML
struct VertexGML {
    glm::vec3 position;
};



// Función para imprimir un vértice (puede ser de tamaño 2, 3 o 4)
void printVertex(const glm::vec4& vertex);

// Función para modificar el punto en el framebuffer
inline void point(const glm::vec3& vector);

void line(const glm::vec3& start, const glm::vec3& end);

void triangle(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C);

void triangulateAndDrawCube(const std::vector<glm::vec3>& vertices, const std::vector<Face>& faces);


