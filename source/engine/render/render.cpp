#include "render.h"
#include "framebuffer.h"
#include "mesh.h"
#include "shaders.h"
#include <vector>

namespace Render {
  struct DrawCall {
    const Mesh& mesh;
    Shaders::ShaderGroup& shaders;
    glm::mat4 transform;
    std::vector<UniformCall> uniforms;
    std::vector<SSBOCall> ssbos;
  };

  std::vector<DrawCall> solid_calls;
  std::vector<DrawCall> trans_calls;

  Framebuffer solid_framebuffer;
  Framebuffer trans_framebuffer;

  void setup(){
    //Build framebuffers
    solid_framebuffer = Framebuffer(0, true, 8);
    solid_framebuffer.attachColor(8, GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT);

    trans_framebuffer = Framebuffer(0, false, 8);
    trans_framebuffer.attachColor(8, GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT).bind(1);
    trans_framebuffer.attachColor(8, GL_R8, GL_RED).bind(2);

    auto& depth_buffer = solid_framebuffer.getDepthBuffer();
    trans_framebuffer.attachDepthBuffer(depth_buffer);
  }

  void addDrawCall(const Mesh& mesh, Shaders::ShaderGroup& shaders, const glm::mat4 transform,
                   const std::vector<UniformCall>& uniforms, const std::vector<SSBOCall>& ssbos){
    mesh.getFlags() & MESH_TRANSPARENT ?
    trans_calls.emplace_back(mesh, shaders, transform, uniforms, ssbos) :
    solid_calls.emplace_back(mesh, shaders, transform, uniforms, ssbos);
  }

  void drawCall(Shader& shader, const Mesh& mesh, const glm::mat4& transform, const std::vector<UniformCall>& uniforms, const std::vector<SSBOCall>& ssbos){
    shader.setUniform("model", transform);
    for(auto& uniform : uniforms){
      shader.setUniform(uniform.name, uniform.value);
    }
    for(auto& ssbo : ssbos){
      ssbo.ssbo.bind();
      ssbo.ssbo.bindBase(0);
    }
    mesh.bind(shader);
    glDrawElements(mesh.getMode(), mesh.size(), GL_UNSIGNED_INT, 0);
  }

  void render(GLFWwindow* window){
    //Draw solid meshes
    solid_framebuffer.bind();
    solid_framebuffer.clear();
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    for(auto& call : solid_calls)
      drawCall(*call.shaders.solid, call.mesh, call.transform, call.uniforms, call.ssbos);

    //Draw transparent meshes
    trans_framebuffer.bind();
    trans_framebuffer.clearBuffer(0, 0);
    trans_framebuffer.clearBuffer(1, 1);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunci(0, GL_ONE, GL_ONE);
    glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
    glBlendEquationi(0, GL_FUNC_ADD);
    glBlendEquationi(1, GL_FUNC_ADD);
    for(auto& call : trans_calls)
      drawCall(*call.shaders.transparent, call.mesh, call.transform, call.uniforms, call.ssbos);

    //Composite pass
    solid_framebuffer.bind();
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawCall(Shaders::composite, Mesh::Square(), 1, {}, {});

    //Draw to screen
    Framebuffer::bind(0);
    Framebuffer::blit(solid_framebuffer, 0);
    glfwSwapBuffers(window);

    solid_calls.clear();
    trans_calls.clear();
  }
}
