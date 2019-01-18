#pragma once

#include <string>
#include <glm/glm.hpp>

namespace kd
{

static bool Vsync = true;
static std::string ASSET_PATH = "assets";
static glm::vec2 WINDOW_SIZE = glm::vec2(800, 600);
static glm::vec3 UI_CLEAR_COLOR = glm::vec3(0.2f, 0.2f, 0.2f);
static bool Line_Polygon = true;
static bool Cull_Back_Face = false;

} // namespace kd