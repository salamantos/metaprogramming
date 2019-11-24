#include <iostream>
#include <string>
#include "task4.h"

int function(size_t) {
    return -146;
}
class Y {};
class X {
public:
    double process(Y& y) {
        return 0.146;
    }
};


int main() {
    // обычные функции
    auto functor1 = make_functor(&function, 146);
    std::cout << functor1.call() << std::endl;

    // функции-члены
    auto functor2 = make_functor(&X::process, Y());
    X x;
    std::cout << functor2.call(x) << std::endl;

    // лямбды
    Y y;
    auto functor3 = make_functor([](Y*) {return 147;}, y);
    std::cout << functor3.call() << std::endl;
}