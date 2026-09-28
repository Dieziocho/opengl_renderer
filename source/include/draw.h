#pragma once
#include "model.h"
#include "shaders.h"

void drawMesh(const Mesh& mesh, Shaders::ShaderGroup& shaders, const glm::mat4& transform = 1);
void drawModel(const Model& model, Shaders::ShaderGroup& shaders, const glm::mat4& transform,
               const std::vector<UniformCall>& uniforms = {}, const std::vector<SSBOCall>& ssbos = {});
