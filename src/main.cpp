#include <iostream>
#include <array>
#include <string>

#include "convert.hpp"
#include "ternary_math.hpp"
#include "hexad.hpp"
#include "word.hpp"
#include "cpu.hpp"
#include "detail/convert_impl.hpp"
#include "detail/character_to_value.hpp"

#include "assembler/assembler.hpp"
#include "assembler/detail/state.hpp"

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
    using namespace std::string_literals;

    std::string sample_filename { "sandbox/sample.txt" };
    ternary::assembler::Assembler as {};

    Cpu cpu {};

    Opcode o1 { 9, 10, 0, 0, 1, -1 };
    cpu.debug_set_memory(26, -123);
    cpu.debug_set_memory(27, 123);
    cpu.debug_set_memory(28, 42);
    std::cout << cpu.get_memory(26).value_string() << '\n';
    cpu.debug_decode_instruction(o1);
    std::cout << cpu.get_register(10) << '\n';
    cpu.debug_print_flags();

    Opcode o2 { -9, 10, 0, 0, 0, -9 };
    cpu.debug_decode_instruction(o2);
    std::cout
        << cpu.get_memory(-9).value_string() << ' '
        << cpu.get_memory(-8).value_string() << ' '
        << cpu.get_memory(-7).value_string() << '\n';
}
