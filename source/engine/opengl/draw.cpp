#include "draw.h"
#include "render.h"

void drawMesh(const Mesh& mesh, Shaders::ShaderGroup& shaders, const glm::mat4& transform){
  Render::addDrawCall(mesh, shaders, transform, {}, {});
}

void drawModel(const Model& model, Shaders::ShaderGroup& shaders, const glm::mat4& transform,
               const std::vector<UniformCall>& uniforms, const std::vector<SSBOCall>& ssbos){
  for(auto& instance : model.mesh_instances)
    Render::addDrawCall(model.meshes[instance.index], shaders, transform * instance.transformation, uniforms, ssbos);
}
