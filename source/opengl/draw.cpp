#include "draw.h"
#include "render.h"

void drawMesh(const Mesh& mesh, Shaders::ShaderGroup& shaders, glm::mat4 transform){
  Render::addDrawCall(mesh, shaders, transform);
}
