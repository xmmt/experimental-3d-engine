#pragma once

namespace Engine {
namespace Utils {

class WindowResolution {
public:
    constexpr WindowResolution(int width, int height) noexcept
        : width_(width)
        , height_(height) {
    }

    [[nodiscard]] constexpr int width() const noexcept {
        return width_;
    }
    [[nodiscard]] constexpr int height() const noexcept {
        return height_;
    }

private:
    int width_;
    int height_;
};

} // namespace Utils
} // namespace Engine