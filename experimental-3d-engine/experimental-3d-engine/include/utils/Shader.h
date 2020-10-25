#pragma once

#include <cassert>
#include <glad/gl.h>

#include <string_view>

namespace Engine {

enum class ShaderType : GLenum {
    VertexShader = GL_VERTEX_SHADER,
    FragmentShader = GL_FRAGMENT_SHADER,
};

template <ShaderType Type>
class Shader {
public:
    Shader(std::string_view const& shaderSource)
        : shaderSource_(shaderSource.data())
        , type_(static_cast<GLenum>(Type)) {
        shader_ = glCreateShader(type_);
        assert(shader_ != 0 && "Can't create shader");
        glShaderSource(shader_, 1, &shaderSource_, nullptr);
        glCompileShader(shader_);
    }
    ~Shader() {
        glDeleteShader(shader_);
    }
    GLuint get() {
        return shader_;
    }

private:
    char const* shaderSource_{ nullptr };
    GLuint shader_{ 0 };
    GLenum const type_{ 0 };
};

} // namespace Engine