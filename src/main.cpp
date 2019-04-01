#include <iostream>
#include <array>

#include "convert.hpp"
#include "ternary_math.hpp"
#include "hexad.hpp"
#include "word.hpp"
#include "opcode.hpp"

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
    Opcode n { 1234567890 };

    std::cout << n.get() << '\n';
    std::cout << n.o_field() << '\t' << n.m_field() << '\t' << n.t_field() << '\t'
              << n.x_field() << '\t' << n.y_field() << '\t' << n.z_field() << '\n';
    std::cout << n.low6() << '\t' << n.low9() << '\t' << n.low12() << '\n';
}
