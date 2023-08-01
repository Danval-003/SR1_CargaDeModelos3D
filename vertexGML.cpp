#include "framebuffer.h"
#include "vertexGML.h"
#include <iostream>


// Función para imprimir un vértice (puede ser de tamaño 2, 3 o 4)
void printVertex(const glm::vec4& vertex) {
    std::cout << "Vertex: (" << vertex.x << ", " << vertex.y;
    if (vertex.length() > 2) {
        std::cout << ", " << vertex.z;
    }
    if (vertex.length() > 3) {
        std::cout << ", " << vertex.w;
    }
    std::cout << ")" << std::endl;
}


// Función para modificar el punto en el framebuffer
inline void point(const glm::vec3& vector) {
    int x = static_cast<int>(vector.x)+ pantallaAncho/4;
    int y = static_cast<int>(vector.y)+ pantallaAlto/2;

    if (x >= 0 && x < pantallaAncho && y >= 0 && y < pantallaAlto) {
        // Modificamos el punto en el framebuffer
        framebuffer[y * pantallaAncho + x] = currentColor;
    }
}


// En vertexGML.cpp, modifica la función line() para que tome en cuenta las coordenadas z de los vértices
void line(const glm::vec3& start, const glm::vec3& end) {
    const int multiply = 120;
    int x1 = round(start.x * multiply), y1 = round(start.y * multiply);
    int x2 = round(end.x * multiply), y2 = round(end.y * multiply);
    float z1 = start.z * multiply, z2 = end.z *multiply;

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        float z = ((dx - abs(err)) * z1 + abs(err) * z2) / dx;
        point(glm::vec3(x1, y1, z));

        if (x1 == x2 && y1 == y2) {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}




void triangle(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C) {
    line(A, B);
    line(B, C);
    line(C, A);
}



void triangulateAndDrawCube(const std::vector<glm::vec3>& vertices, const std::vector<Face>& faces) {
    for (const auto& face : faces) {
        if (face.vertexIndices.size() >= 3) {
            // Tomamos los tres primeros vértices de cada cara para triangularla
            const glm::vec3& A = vertices[face.vertexIndices[0][0] - 1];
            const glm::vec3& B = vertices[face.vertexIndices[1][0] - 1];
            const glm::vec3& C = vertices[face.vertexIndices[2][0] - 1];

            // Triangular la cara utilizando los vértices A, B y C
            triangle(A, B, C);

        }
    }
}


