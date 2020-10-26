#pragma once

#include <memory>

namespace Engine {

template <typename Unused>
class Function;

template <typename ReturnType, typename... Args>
class Function<ReturnType(Args...)> {
public:
    Function() = default;

    template <typename F>
    Function(F f)
        : invoker_(std::make_unique<FunctionHolder<F>>(std::forward<F>(f))) {
    }
    ReturnType operator()(Args... args) {
        return invoker_->invoke(std::forward<Args>(args)...);
    }
    operator bool() const {
        return invoker_.operator bool();
    }

private:
    class FunctionHolderBase {
    public:
        FunctionHolderBase() = default;
        FunctionHolderBase(FunctionHolderBase const&) = delete;
        FunctionHolderBase(FunctionHolderBase&&) = delete;
        void operator=(FunctionHolderBase const&) = delete;
        void operator=(FunctionHolderBase&&) = delete;
        virtual ~FunctionHolderBase() = default;
        virtual ReturnType invoke(Args... args) = 0;
    };
    using invoker_t = std::unique_ptr<FunctionHolderBase>;

    template <typename F>
    class FunctionHolder final : public FunctionHolderBase {
    public:
        FunctionHolder(F f)
            : FunctionHolderBase()
            , function_(std::forward<F>(f)) {
        }
        ReturnType invoke(Args... args) final {
            return function_(std::forward<Args>(args)...);
        }

    private:
        F function_;
    };

    invoker_t invoker_;
};

} // namespace Engine