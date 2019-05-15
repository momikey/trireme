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

    cpu.debug_set_register(-6, -3);
    cpu.debug_set_register(1, -777);

    Opcode o1 { 10, 4, 1, 2, 3, 4 };    // bra %ABCD
    Opcode o2 { 10, 2, 0, 1, 0, 0 };    // bri rA, 0
    Opcode o3 { 10, -9, 0, 1, 2, -6};   // car +777
    Opcode o4 { 10, -13, 0, 0, 0, 0};   // ret

    cpu.debug_decode_instruction(o1);
    std::cout << cpu.get_instruction_pointer() << '\n';
    cpu.debug_print_flags();

    cpu.debug_decode_instruction(o2);
    std::cout << cpu.get_instruction_pointer() << '\n';
    cpu.debug_print_flags();

    cpu.debug_decode_instruction(o3);
    std::cout << cpu.get_instruction_pointer() << '\n';
    std::cout << cpu.get_register(-6) << '\n';
    std::cout << cpu.get_memory_word(-3) << '\n';
    cpu.debug_print_flags();

    cpu.debug_decode_instruction(o4);
    std::cout << cpu.get_instruction_pointer() << '\n';
    std::cout << cpu.get_register(-6) << '\n';
    std::cout << cpu.get_memory_word(-3) << '\n';
    cpu.debug_print_flags();
}
