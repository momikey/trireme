#include <iostream>

#include "convert.h"

int main(int, char**) {
    auto testvalues = {0, 1, 2, 4, 8, -5, -12, -13};
    for (auto&& v : testvalues)
    {
        std::cout << lowest_trit(v) << '\n';
    }

    auto powtest = {0u, 1u, 2u, 6u, 9u, 12u, 18u};
    for (auto&& v : powtest)
    {
        std::cout << pow3(v) << '\n';
    }
}
