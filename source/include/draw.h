#pragma once
#include "mesh.h"
#include "shaders.h"

void drawMesh(const Mesh& mesh, Shaders::ShaderGroup& shaders, glm::mat4 transform = 1);
