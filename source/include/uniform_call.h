#pragma once
#include "ssbo_class.h"
#include "glm/ext/matrix_float4x4.hpp"
#include <variant>

struct UniformValue {
  std::variant<
    int,
    unsigned,
    float,
    glm::vec2,
    glm::vec3,
    glm::vec4,
    glm::mat4
    > value;
};

struct UniformCall {
  const char* name;
  UniformValue value;

  template<typename T>
  UniformCall(const char* name, T value) : name(name), value{value}{}
};

struct SSBOCall {
  SSBO& ssbo;
  unsigned base;

  SSBOCall(SSBO& ssbo, unsigned base) : ssbo(ssbo), base(base){}
};
