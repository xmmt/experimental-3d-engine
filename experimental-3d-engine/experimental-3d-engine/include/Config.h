#pragma once

#include <utils/OpenGLVersion.hpp>
#include <utils/WindowResolution.hpp>

#include <string_view>

namespace Engine {
namespace Config {

constexpr const std::string_view droidSansPath{ "../../../../experimental-3d-engine/imgui/misc/fonts/DroidSans.ttf" };

constexpr const Utils::OpenGLVersion defaultOpenGLVersion{ 4, 6, 460 };

constexpr const Utils::WindowResolution defaultWindowResolution{ 800, 600 };

constexpr const std::string_view defaultWindowTitle{ "Window" };

} // namespace Config
} // namespace Engine
