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
        { -8, -1, 3, 0, 9,  0 },    // out rC, @%I0
        { -8,  1, 1, 0, 9, -1 },    // int @%In, rA
        { -8, -1, 1, 0, 9, -2 },    // out rA, @%Io
        { -8, -1, 2, 0, 9, -2 },    // out rB, @%Io
    };

    // for (auto& o : opcodes)
    // {
    //     cpu.debug_decode_instruction(o);
    //     std::cout << cpu.get_instruction_pointer() << '\n';
    //     cpu.debug_print_flags();
    // }

    auto data { as.assemble_file(sample_filename) };
    
    for (auto& d : data)
    {
        std::cout << d.first << '\t' << d.second << '\n';
    }
}
