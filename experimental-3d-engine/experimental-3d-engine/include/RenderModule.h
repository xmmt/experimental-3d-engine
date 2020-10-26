#pragma once

#include <utils/FunctionHolder.hpp>

namespace Engine {

class RenderModule {
public:
    RenderModule(Function<Function<void(int, int)>()>&& initFunction)
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
    Function<Function<void(int, int)>()> initFunction_;
    Function<void(int, int)> drawFunction_;
};

} // namespace Engine