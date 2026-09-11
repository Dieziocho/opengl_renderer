#pragma once
#include "opengl.h"
#include "file_utils.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include <filesystem>

class Shader {
public:
  using Path = std::filesystem::path;

  Shader() = default;
  Shader(Path vertex_shader, Path fragment_shader) :
    id(compileShader(vertex_shader, fragment_shader)){}

  void use() const {
    glUseProgram(this->id);
  }

  template<typename T>
  void setUniform(const std::string &name, const T& value){
    auto location = glGetUniformLocation(id, name.c_str());

    this->use();
    if constexpr(std::is_same_v<T, int>)  glUniform1i(location, value);
    else if constexpr(std::is_same_v<T, unsigned>)  glUniform1i(location, value);
    else if constexpr(std::is_same_v<T, float>)     glUniform1f(location, value);
    else if constexpr(std::is_same_v<T, glm::vec2>) glUniform2fv(location, 1, glm::value_ptr(value));
    else if constexpr(std::is_same_v<T, glm::vec3>) glUniform3fv(location, 1, glm::value_ptr(value));
    else if constexpr(std::is_same_v<T, glm::vec4>) glUniform4fv(location, 1, glm::value_ptr(value));
    else if constexpr(std::is_same_v<T, glm::mat4>) glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    else{
      static_assert(false, "Invalid uniform type");
    }
  }

  unsigned getId(){
    return id;
  }

private:
  unsigned id;

  static unsigned compileShader(Path vertex_path, Path fragment_path){
    int success;
    char info_log[512];

    //Compile vertex
    const char* vertex_source = readFile(vertex_path);
    unsigned vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);
    //Check success
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success){
      glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
      throw std::runtime_error(std::format("Failed to compile vertex shader {}: {}\n", vertex_path, info_log));
    }
    delete[] vertex_source;

    //Compile fragment
    auto fragment_source = readFile(fragment_path);
    unsigned fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);
    //Check success
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success){
      glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
      throw std::runtime_error(std::format("Failed to compile fragment shader {}: {}\n", fragment_path, info_log));
    }
    delete[] fragment_source;

    //Link shaders into shader program
    unsigned shader_id = glCreateProgram();
    glAttachShader(shader_id, vertex_shader);
    glAttachShader(shader_id, fragment_shader);
    glLinkProgram(shader_id);
    glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
    if(!success){
      glGetProgramInfoLog(shader_id, 512, NULL, info_log);
      throw std::runtime_error(std::format("Failed to link shaders {} {}: {}\n", vertex_path, fragment_path, info_log));
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_id;
  }
};
