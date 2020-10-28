#pragma once

#include <memory>

namespace Engine {

template <typename Unused>
class Function;

template <typename ReturnType, typename... Args>
class Function<ReturnType(Args...)> {
private:
    class FunctionHolderBase {
    public:
        FunctionHolderBase() = default;
        FunctionHolderBase(FunctionHolderBase const&) = delete;
        FunctionHolderBase(FunctionHolderBase&&) = delete;
        void operator=(FunctionHolderBase const&) = delete;
        void operator=(FunctionHolderBase&&) = delete;
        virtual ~FunctionHolderBase() = default;

        virtual ReturnType invoke(Args&&...) = 0;
    };
    using invoker_t = std::unique_ptr<FunctionHolderBase>;

    template <typename F>
    class FunctionHolder final : public FunctionHolderBase {
    public:
        FunctionHolder(F&& f)
            : FunctionHolderBase()
            , function_(std::forward<F>(f)) {
        }
        void operator=(FunctionHolder const&) = delete;
        void operator=(FunctionHolder&&) = delete;
        ~FunctionHolder() = default;

        ReturnType invoke(Args&&... args) final {
            return function_(std::forward<Args>(args)...);
        }

    private:
        F function_;
    };

    invoker_t invoker_;

public:
    Function() = default;
    Function(Function const&) = delete;
    Function(Function&&) = default;
    Function& operator=(Function const&) = delete;
    Function& operator=(Function&&) = default;

    template <typename F>
    Function(F const& f)
        : invoker_(std::make_unique<FunctionHolder<F>>(f)) {
    }
    template <typename F>
    Function(F&& f)
        : invoker_(std::make_unique<FunctionHolder<F>>(std::forward<F>(f))) {
    }

    inline ReturnType operator()(Args... args) {
        return invoker_->invoke(std::forward<Args>(args)...);
    }
    inline operator bool() const {
        return invoker_.operator bool();
    }
};

} // namespace Engine