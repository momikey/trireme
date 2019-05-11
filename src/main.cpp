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

    Opcode o1 { 4, 13, -13, 1, 2, -10 };    // adi rA, rz, 44
    Opcode o2 { 4, -8, 1, 0, 0, 13 };    // psb r!, 13

    cpu.debug_decode_instruction(o1);
    std::cout << cpu.get_register(1) << '\n';
    cpu.debug_print_flags();

    cpu.debug_decode_instruction(o2);
    std::cout << cpu.get_register(1) << '\n';
    cpu.debug_print_flags();

}
