#pragma once

template <typename Signature>
class function_ref;

template <typename Return, typename... Args>
class function_ref<Return(Args...)>
{
    using storage = const void*;
    using callback = Return (*)(const void*, Args...);

    storage st;
    callback cb;

public:
    function_ref(Return (*f)(Args...))
    {
        this->st = static_cast<const void*>(f);
        this->cb = [](const void* obj, Args... args) {
            auto func = *static_cast<Return (*)(Args...)>(obj);
            return static_cast<Return>(func(static_cast<Args>(args)...));
        };
    }

    template <typename Callable>
    function_ref(const Callable& f)
    {
        this->st = static_cast<const void*>(&f);
        this->cb = [](const void* obj, Args... args) {
            auto& func = *static_cast<const Callable*>(obj);
            return static_cast<Return>(func(static_cast<Args>(args)...));
        };
    }

    Return operator()(Args... args) const { return this->cb(this->st, static_cast<Args>(args)...); }
};
