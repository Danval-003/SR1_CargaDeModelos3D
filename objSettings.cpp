#include "objSettings.h"
#include <iostream>
#include "vertexGML.h"
using namespace std;


bool loadOBJ(const std::string& path, std::vector<glm::vec3>& out_vertices, std::vector<Face>& out_faces) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open OBJ file: " << path << std::endl;
        return false;
    }

    std::vector<glm::vec3> temp_vertices;
    std::vector<std::vector<int>> temp_faces;

    std::string line;
    while (std::getline(file, line)) {

        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {

            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        } else if (type == "f") {
            std::vector<int> face_indices;
            int vertex_index, texture_index, normal_index;
            char slash;
            while (iss >> vertex_index >> slash >> texture_index >> slash >> normal_index) {
                face_indices.push_back(vertex_index);
            }
            temp_faces.push_back(face_indices);
        }
    }
    // Set the output vertices
    out_vertices = temp_vertices;
    for (const auto& faced : temp_faces) {
        out_faces.push_back(Face(faced));
    }

    return true;
}

bool isPointInsideTriangle(const glm::vec3& point, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) {
    // Calculate vectors from each vertex to the point
    glm::vec3 v0ToPoint = point - v0;
    glm::vec3 v1ToPoint = point - v1;
    glm::vec3 v2ToPoint = point - v2;

    // Calculate dot products
    float dot00 = glm::dot(v0ToPoint, v0ToPoint);
    float dot01 = glm::dot(v0ToPoint, v1ToPoint);
    float dot02 = glm::dot(v0ToPoint, v2ToPoint);
    float dot11 = glm::dot(v1ToPoint, v1ToPoint);
    float dot12 = glm::dot(v1ToPoint, v2ToPoint);

    // Calculate barycentric coordinates
    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is inside the triangle
    return (u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.0f);
}

void triangulateFaces(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& outVertices, std::vector<Face>& out_faces) {
    // The output vertices are the same as the input vertices
    outVertices = vertices;

    for (const auto& face : faces) {
        const int numVertices = face.vertexIndices.size();
        if (numVertices < 3) {
            std::cerr << "Warning: Face with less than 3 vertices. Skipping." << std::endl;
            continue;
        }

        if (numVertices == 3) {
            // If the face is already a triangle, add it directly to the output faces
            out_faces.push_back(face);
        } else {
            // Triangulate the face using ear-clipping algorithm
            std::vector<int> remainingVertices = face.vertexIndices;
            while (remainingVertices.size() > 3) {
                bool earFound = false;

                for (int i = 0; i < remainingVertices.size(); ++i) {
                    int prevIdx = (i == 0) ? remainingVertices.size() - 1 : i - 1;
                    int nextIdx = (i + 1) % remainingVertices.size();

                    int prevVertexIndex = remainingVertices[prevIdx];
                    int currentVertexIndex = remainingVertices[i];
                    int nextVertexIndex = remainingVertices[nextIdx];

                    glm::vec3 prevVertex = vertices[prevVertexIndex];
                    glm::vec3 currentVertex = vertices[currentVertexIndex];
                    glm::vec3 nextVertex = vertices[nextVertexIndex];

                    bool isEar = true;

                    // Check if the current vertex forms an ear
                    for (int j = 0; j < remainingVertices.size(); ++j) {
                        if (j != prevIdx && j != i && j != nextIdx) {
                            int testVertexIndex = remainingVertices[j];
                            glm::vec3 testVertex = vertices[testVertexIndex];

                            // If the test vertex is inside the triangle, the current vertex is not an ear
                            if (isPointInsideTriangle(testVertex, prevVertex, currentVertex, nextVertex)) {
                                isEar = false;
                                break;
                            }
                        }
                    }

                    if (isEar) {
                        earFound = true;
                        std::vector<int> ho = {prevVertexIndex, currentVertexIndex, nextVertexIndex};
                        // Add the triangle formed by the current ear to the output faces
                        out_faces.push_back(Face(ho));

                        // Remove the current vertex from the remaining vertices
                        remainingVertices.erase(remainingVertices.begin() + i);

                        // Start from the beginning to check the ears again
                        break;
                    }
                }

                // If no ear is found, the polygon may be self-intersecting or have other issues
                // In this case, stop triangulating and output the remaining vertices as a triangle
                if (!earFound) {
                    std::cerr << "Warning: Unable to find ear in polygon. Triangulation may not be valid." << std::endl;
                    std::vector<int> hola = {remainingVertices[0], remainingVertices[1], remainingVertices[2]};
                    out_faces.push_back(Face(hola));
                    break;
                }
            }

            // Add the last triangle formed by the remaining vertices (should be a triangle)
            std::vector<int> h2 = {remainingVertices[0], remainingVertices[1], remainingVertices[2]};
            out_faces.push_back(Face(h2));
        }
    }
}


