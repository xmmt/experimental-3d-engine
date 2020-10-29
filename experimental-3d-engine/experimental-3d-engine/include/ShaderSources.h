#pragma once

#include <string_view>

namespace ShaderSources {
using namespace std::string_view_literals;

constexpr const auto vertexShaderSource =
  R"shader(#version 460 core

layout(location = 0) in vec3 position;

void main() {
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
}
)shader"sv;

constexpr const auto fragmentShaderSource =
  R"shader(#version 460 core

out vec4 color;

void main() {
	color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)shader"sv;

constexpr const auto textureFragmentShaderSource =
  R"shader(#version 460 core

out vec4 color;

void main() {
	color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}
)shader"sv;

} // namespace ShaderSources
