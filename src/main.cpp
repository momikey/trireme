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

    cpu.debug_set_memory(3, 4);
    cpu.debug_set_memory(4, 12);
    cpu.debug_set_memory(5, -7);

    Opcode o0 { 8, 0, 0, 1, 0, 3 }; // ldi 3, rA
    cpu.debug_set_register(4, 4);
    cpu.debug_set_register(5, 5);

    Opcode o1 { 8, 5, 0, 0, 1, 2 }; // lal [rA], rB
    Opcode o2 { 8, 6, 0, 0, 4, 2 }; // lam [rD], rB
    Opcode o3 { 8, 7, 0, 0, 5, 2 }; // lah [rE], rB
    Opcode o4 { 8, 9, 0, 0, 1, 3 }; // lda [rA], rC

    cpu.debug_decode_instruction(o0);

    cpu.debug_decode_instruction(o1);
    std::cout << cpu.get_register(2) << '\n';
    cpu.debug_print_flags();

    cpu.debug_decode_instruction(o2);
    std::cout << cpu.get_register(2) << '\n';
    cpu.debug_print_flags();

    cpu.debug_decode_instruction(o3);
    std::cout << cpu.get_register(2) << '\n';
    cpu.debug_print_flags();

    cpu.debug_set_flag(flags::direction, 1);

    cpu.debug_decode_instruction(o4);
    std::cout << cpu.get_register(3) << '\n';
    std::cout << cpu.get_register(1) << '\n';
    cpu.debug_print_flags();
}
