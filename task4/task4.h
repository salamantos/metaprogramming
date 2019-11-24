template<class ...T>
class ArgsPack;

template<>
class ArgsPack<> {
public:
    ArgsPack() = default;

    template<class Delegate, class...Args>
    auto call(Delegate delegate, Args... args) -> decltype(delegate(args...)) {
        return delegate(args...);
    }
};

template<class Head, class... Tail>
class ArgsPack<Head, Tail...> {
private:
    Head param_;
    ArgsPack<Tail...> base_;

    Head* to_pointer(Head& h) {
        return &h;
    }

    Head* to_pointer(Head* h) {
        return h;
    }

    Head& to_ref(Head& h) {
        return h;
    }

    Head& to_ref(Head* h) {
        return *h;
    }

public:
    explicit ArgsPack(Head head, Tail... tail) : base_(tail...), param_(head) {}

    template<class Delegate, class...Args>
    auto call(const Delegate& delegate, Args... args)
    -> decltype(base_.call(delegate, args..., to_ref(this->param_))) {
        return base_.call(delegate, args..., to_ref(param_));
    }

    template<class Delegate, class...Args>
    auto call(const Delegate& delegate, Args... args)
        -> decltype(base_.call(delegate, args..., to_pointer(this->param_))) {
        return base_.call(delegate, args..., to_pointer(param_));
    }
};


template<class Function>
class FunctionWrapper {
private:
    Function callable_func_;

public:
    explicit FunctionWrapper(const Function& func_pointer) : callable_func_(func_pointer) {}

    template<class... Args>
    auto operator()(Args... args) -> decltype(callable_func_(args...)) {
        return callable_func_(args...);
    }
};

template<class Object, class Function>
class SimpleDelegate {

private:
    Object& object_;
    Function callable_func_;

public:
    SimpleDelegate(Object& object, const Function& func_pointer) : object_(object), callable_func_(func_pointer) {}

    template<class... Args>
    auto operator()(Args... args) -> decltype((object_.*callable_func_)(args...)) {
        return (object_.*callable_func_)(args...);
    }
};

template<class Function, class... Args>
class Functor {
private:
    typedef ArgsPack<Args...> base;
    Function callable_func_;
    base base_;

public:
    explicit Functor(const Function& callable_func_, Args...args) : base_(args...), callable_func_(callable_func_) {}

    template<class Callable = Function>
    auto call() -> decltype(base_.call(FunctionWrapper<Callable>(callable_func_))) {
        return base_.call(FunctionWrapper<Callable>(callable_func_));
    }

    template<class Object>
    auto call(Object& object) -> decltype(base_.call(SimpleDelegate<Object, Function>(object, callable_func_))) {
        return base_.call(SimpleDelegate<Object, Function>(object, callable_func_));
    }
};

template<class Function, class... Args>
Functor<Function, Args...> make_functor(Function function, Args...args) {
    return Functor<Function, Args...>(function, args...);
}