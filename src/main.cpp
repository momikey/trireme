#include <iostream>
#include <array>

#include "convert.hpp"
#include "ternary_math.hpp"
#include "hexad.hpp"
#include "word.hpp"

using namespace ternary;

template<typename T, std::size_t N>
void print_array(const std::array<T,N>& arr)
{
    for (auto&& v : arr)
    {
        std::cout << v << ',';
    }
}

std::ostream& operator<<(std::ostream& os, Hexad h)
{
    return os << "H" << h.get();
}

std::ostream& operator<<(std::ostream& os, Word w)
{
    return os << w.value_string();
}

template<typename T, typename U>
std::ostream& operator<<(std::ostream& os, std::pair<T, U> pair)
{
    return os << pair.first << ',' << pair.second;
}

int main(int, char**) {
    Word ww1 { 12345 };
    Word ww2 { -1234 };

    auto big = (long long)ww1.value() * ww2.value() * ww2.value();

    print_array(to_trits<long long, 24>(big));
    std::cout << mul(ww1,ww2.value() * ww2.value()) << '\n';

    auto dr { div((ww1), sti(ww2)) };

    std::cout << ww1.value() / ww2.value() << '\t' << dr << '\t'
        << dr.first.value() * 1234 + dr.second.value() << '\n';
    std::cout << div({ww2, ww2}, ww1) << '\n';
    std::cout << (ww2.value() * pow3<long long>(Word::word_size) + ww2.value()) / ww1.value() << '\n';
}
