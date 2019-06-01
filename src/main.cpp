#include <iostream>
#include <array>
#include <string>
#include <cstdint>

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

    cpu.debug_set_register(1, 'x');
    cpu.debug_set_register(2, 0);
    cpu.debug_set_register(3, 1);

    Opcode opcodes[] {
        { 8, 0, 0, 1, 0, 0 },   // ldi 0, rA
        { 8, 0, 0, 2, 4, 4 },   // ldi %DD, rB
        { 0, 0, 0, 0, 0, 0 }    // und
    };

    for (auto& o : opcodes)
    {
        cpu.debug_decode_instruction(o);
        std::cout << cpu.get_instruction_pointer() << '\n';
        cpu.debug_print_flags();
    }

    // auto data { as.assemble_file(sample_filename) };
    
    // for (auto& d : data)
    // {
    //     std::cout << d.first << '\t' << d.second << '\n';
    // }
}
