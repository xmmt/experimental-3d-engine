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
    Shader() = default;
    Shader(Shader const&) = delete;
    Shader(std::string_view shaderSource)
        : shaderSource_(shaderSource.data())
        , type_(static_cast<GLenum>(Type)) {
        shader_ = glCreateShader(type_);
        assert(shader_ != 0 && "Can't create shader");
        glShaderSource(shader_, 1, &shaderSource_, nullptr);
        glCompileShader(shader_);
    }
    Shader(Shader&& other)
        : shaderSource_(other.shaderSource_)
        , shader_(other.shader_)
        , type_(other.type_) {
        other.shader_ = 0;
    }
    Shader& operator=(Shader&& other) {
        shaderSource_ = other.shaderSource_;
        shader_ = other.shader_;
        type_ = other.type_;
        other.shader_ = 0;
        return *this;
    }
    ~Shader() {
        if (shader_ != 0) {
            glDeleteShader(shader_);
        }
    }
    GLuint get() {
        return shader_;
    }

private:
    char const* shaderSource_{ nullptr };
    GLuint shader_{ 0 };
    GLenum type_{ 0 };
};

enum class ProgramState {
    RequiersVertexShader,
    RequiersFragmentShader,
    RequiersLinking,
    ReadyToUse,
};

template <ProgramState State>
class Program;

Program<ProgramState::RequiersVertexShader> makeShaderProgram();

template <ProgramState State>
class Program {
public:
    friend Program<ProgramState::RequiersVertexShader> makeShaderProgram();

    Program(Program const&) = delete;
    Program operator=(Program const&) = delete;
    Program operator=(Program&&) = delete;

    friend class Program<ProgramState::RequiersVertexShader>;
    friend class Program<ProgramState::RequiersFragmentShader>;
    friend class Program<ProgramState::RequiersLinking>;
    friend class Program<ProgramState::ReadyToUse>;
    Program(Program<ProgramState::RequiersVertexShader>&& other)
        : program_(other.program_)
        , vertexShader_(std::move(other.vertexShader_))
        , fragmentShader_(std::move(other.fragmentShader_)) {
        static_assert(State == ProgramState::RequiersVertexShader || State == ProgramState::RequiersFragmentShader);
        other.program_ = 0;
    }
    Program(Program<ProgramState::RequiersFragmentShader>&& other)
        : program_(other.program_)
        , vertexShader_(std::move(other.vertexShader_))
        , fragmentShader_(std::move(other.fragmentShader_)) {
        static_assert(State == ProgramState::RequiersFragmentShader || State == ProgramState::RequiersLinking);
        other.program_ = 0;
    }
    Program(Program<ProgramState::RequiersLinking>&& other)
        : program_(other.program_)
        , vertexShader_(std::move(other.vertexShader_))
        , fragmentShader_(std::move(other.fragmentShader_)) {
        static_assert(State == ProgramState::RequiersLinking || State == ProgramState::ReadyToUse);
        other.program_ = 0;
    }
    Program(Program<ProgramState::ReadyToUse>&& other)
        : program_(other.program_)
        , vertexShader_(std::move(other.vertexShader_))
        , fragmentShader_(std::move(other.fragmentShader_)) {
        static_assert(State == ProgramState::ReadyToUse);
        other.program_ = 0;
    }

    ~Program() {
        if (program_ != 0) {
            glDeleteProgram(program_);
        }
    }

    Program<ProgramState::RequiersFragmentShader>
    addVertexShader(std::string_view vertexShader) && {
        static_assert(State == ProgramState::RequiersVertexShader);
        vertexShader_ = Shader<ShaderType::VertexShader>(vertexShader);
        glAttachShader(program_, vertexShader_.get());
        return { std::move(*this) };
    };

    Program<ProgramState::RequiersLinking>
    addFragmentShader(std::string_view fragmentShader) && {
        static_assert(State == ProgramState::RequiersFragmentShader);
        fragmentShader_ = Shader<ShaderType::FragmentShader>(fragmentShader);
        glAttachShader(program_, fragmentShader_.get());
        return { std::move(*this) };
    }

    Program<ProgramState::ReadyToUse>
    link() && {
        static_assert(State == ProgramState::RequiersLinking);
        glLinkProgram(program_);
        return { std::move(*this) };
    }

    void use() const {
        static_assert(State == ProgramState::ReadyToUse);
        glUseProgram(program_);
    }

private:
    Program() = default;
    Program(GLuint program, Shader<ShaderType::VertexShader>&& vertexShader, Shader<ShaderType::FragmentShader>&& fragmentShader)
        : program_(program)
        , vertexShader_(std::move(vertexShader))
        , fragmentShader_(std::move(fragmentShader)) {
    }

private:
    GLuint program_{ 0 };
    Shader<ShaderType::VertexShader> vertexShader_;
    Shader<ShaderType::FragmentShader> fragmentShader_;
};

inline Program<ProgramState::RequiersVertexShader> makeShaderProgram() {
    return { glCreateProgram(), {}, {} };
}

} // namespace Engine