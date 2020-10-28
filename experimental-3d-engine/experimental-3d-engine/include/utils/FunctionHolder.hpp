#pragma once

#include <memory>

namespace Engine {

template <typename Unused>
class Function;

template <typename ReturnType, typename... Args>
class Function<ReturnType(Args...)> {
private:
    struct FunctionHolderBase {
        virtual ~FunctionHolderBase() = default;
        virtual ReturnType invoke(Args&&...) = 0;
    };
    using invoker_t = std::unique_ptr<FunctionHolderBase>;

    template <typename F>
    struct FunctionHolder final : public FunctionHolderBase {
        F function_;
        FunctionHolder(const F& f)
            : FunctionHolderBase()
            , function_(f) {
        }
        FunctionHolder(F&& f)
            : FunctionHolderBase()
            , function_(std::move(f)) {
        }

        inline ReturnType invoke(Args&&... args) final {
            return function_(std::forward<Args>(args)...);
        }
    };

    mutable invoker_t invoker_;

public:
    Function() = default;

    template <typename F>
    Function(F&& f)
        : invoker_(std::make_unique<FunctionHolder<typename std::decay<F>::type>>(std::forward<F>(f))) {
    }

    inline ReturnType operator()(Args... args) const {
        return invoker_->invoke(std::forward<Args>(args)...);
    }
    inline operator bool() const {
        return invoker_.operator bool();
    }
};

} // namespace Engine