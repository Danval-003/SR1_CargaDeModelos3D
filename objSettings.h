#pragma once

#include "vertexGML.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

bool loadOBJ(const std::string& path, std::vector<glm::vec3>& out_vertices, std::vector<Face>& out_faces);

void triangulateFaces(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& outVertices, std::vector<Face>& out_faces);
