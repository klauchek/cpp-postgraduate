## [RAM build 
```
cmake -S . -B  build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
## [RAM run
```
./build/cram < tests/test.cram
```
# Standard tasks: what is going on?
## task 1:  https://godbolt.org/z/5ajM9W8bK
```
#include <cassert>
template <class T1, class T2>
struct Pair {
    template<class U1 = T1, class U2 = T2>
    Pair(U1&&, U2&&) {}
};

struct S {
    S() = default;
};

struct E { 
    explicit E() = default;
};

int f(Pair<E, E>) {
    return 1; 
}

int f(Pair<S, S>) {
    return 2;
}

int main() {
    assert(f({{}, {}}) == 2); // Error or Assert or O
}
```
 ![Screenshot](pics/compilation1.png)

Based on Proposed resolution (approved by CWG 2024-02-16): Change in 12.2.2.4 [over.match.ctor] paragraph (https://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html 2856. Copy-list-initialization with explicit default constructors)

When objects of class type are direct-initialized, copy-initialized from an expression of the same or a derived class type (our case), or default-initialized, overload resolution selects the constructor.

For direct-initialization or default-initialization (including default-initialization in the context of copy-list-initialization (***OUR CASE***)), the candidate functions are all the constructors of the class of the object being initialized. Otherwise, the candidate functions are all the converting constructors (11.4.8.2 [class.conv.ctor]) of that class. The argument list is the expression-list or assignment-expression of the initializer. For default-initialization in the context of copy-list-initialization, if an explicit constructor is chosen, the initialization is ill-formed (***OUR CASE***).

So, clang is right here. Explicit constructor can't be chosen. 

## task 2: 