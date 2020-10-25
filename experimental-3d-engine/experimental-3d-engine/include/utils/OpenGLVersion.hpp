#pragma once

namespace Engine {
namespace Utils {

class OpenGLVersion {
public:
    constexpr OpenGLVersion(int majorVersion, int minorVersion, int shaderVersion) noexcept
        : majorVersion_(majorVersion)
        , minorVersion_(minorVersion)
        , shaderVersion_(shaderVersion) {
    }

    [[nodiscard]] constexpr int majorVersion() const noexcept {
        return majorVersion_;
    }

    [[nodiscard]] constexpr int minorVersion() const noexcept {
        return minorVersion_;
    }

    [[nodiscard]] constexpr int shaderVersion() const noexcept {
        return shaderVersion_;
    }

private:
    int majorVersion_;
    int minorVersion_;
    int shaderVersion_;
};

} // namespace Utils
} // namespace Engine