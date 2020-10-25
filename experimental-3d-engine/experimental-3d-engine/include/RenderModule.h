#pragma once

#include <functional>

namespace Engine {

class RenderModule {
public:
    RenderModule(std::function<std::function<void(int, int)>()>&& initFunction)
        : initFunction_(std::move(initFunction)) {
    }
    void init() {
        if (initFunction_) {
            drawFunction_ = initFunction_();
        }
    }
    void draw(int width, int height) {
        if (drawFunction_) {
            drawFunction_(width, height);
        }
    }

private:
    std::function<std::function<void(int, int)>()> initFunction_;
    std::function<void(int, int)> drawFunction_;
};

} // namespace Engine