#pragma once

//#include <utils/FunctionHolder.hpp>
#include <functional>

namespace Engine {

template <typename Data>
class RenderModule {
public:
    template <typename F>
    RenderModule(F&& initFunction)
        : initFunction_(std::forward<F>(initFunction)) {
    }
    void init(Data const& data) {
        if (initFunction_) {
            drawFunction_ = initFunction_(data);
        }
    }
    void draw(Data const& data) {
        if (drawFunction_) {
            drawFunction_(data);
        }
    }

private:
    std::function<std::function<void(Data const&)>(Data const&)> initFunction_;
    std::function<void(Data const&)> drawFunction_;
};

} // namespace Engine