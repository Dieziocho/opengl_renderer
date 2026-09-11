#pragma once
#include <cstddef>

inline constexpr size_t window_width = 1366;
inline constexpr size_t window_height = 768;
inline const char* window_name = "testestest";

#define WINDOW_RESOLUTION window_width, window_height
#define BACKGROUND_COLOR 0.2196f, 0.4510f, 0.3843f

inline constexpr float player_speed = 0.1;

inline constexpr float fov = 50.0;
inline constexpr float camera_sensitivity = 0.01;
inline constexpr float camera_target_offset = 0.5;
inline constexpr float camera_near = 0.1;
inline constexpr float camera_far = 10000.0;


#include "glm/ext/vector_float3.hpp"
inline constexpr glm::vec3 world_up(0,1,0);

