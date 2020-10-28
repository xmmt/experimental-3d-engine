#pragma once

#include <utils/FunctionHolder.hpp>

namespace Engine {

template <typename Data>
class RenderModule {
public:
    template<typename F>
    RenderModule(F initFunction)
        : initFunction_(std::move(initFunction)) {
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
    Function<Function<void(Data const&)>(Data const&)> initFunction_;
    Function<void(Data const&)> drawFunction_;
};

} // namespace Engine