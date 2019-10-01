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

#include "shell/shell.hpp"

#include <fmt/core.h>

using namespace ternary;

template<typename T, std::size_t N>
void print_array(const std::array<T,N>& arr)
{
    for (auto&& v : arr)
    {
        std::cout << v << ',';
    }
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
    cpu.reset();

    ternary::shell::Shell shell { cpu };

    shell.start();

    // cpu.debug_set_register(1, 'x');
    // cpu.debug_set_register(2, 0);
    // cpu.debug_set_register(3, 1);

    // Opcode opcodes[] {
    //     { 8, 0, 0, 1, 0, 0 },   // ldi 0, rA
    //     { 8, 0, 0, 2, 4, 4 },   // ldi %DD, rB
    //     { 0, -12, 0, 0,0,0 },   // brk
    //     { 0, 0, 0, 0, 0, 0 }    // und
    // };

    // auto origin { cpu.get_instruction_pointer().value() };
    // while (true)
    // {
    //     std::string line;
    //     std::getline(std::cin, line);

    //     auto result { as.assemble_line(line, origin) };

    //     if (!result.empty())
    //     {
    //         for (auto& d : result)
    //         {
    //             if (d.first >= origin)
    //                 fmt::print("Disassembly {0}\t{1}\n", d.first, d.second);
    //         }

    //         origin += result.size();
    //     }
    // }

    // try
    // {
    //     auto address { cpu.get_instruction_pointer().value() };

    //     for (auto& o : opcodes)
    //     {
    //         // cpu.debug_decode_instruction(o);
    //         // // std::cout << cpu.get_instruction_pointer() << '\n';
    //         // fmt::print("{0}\n", cpu.get_instruction_pointer());
    //         // cpu.debug_print_flags();
    //         auto v { o.value };
    //         cpu.debug_set_memory(address++, v.low().get());
    //         cpu.debug_set_memory(address++, v.middle().get());
    //         cpu.debug_set_memory(address++, v.high().get());
    //     }

    //     cpu.run();
    // }
    // catch (const ternary::system_interrupt_base& e)
    // {
    //     std::cerr << e.value() << '\n';
    // }

    // cpu.debug_print_flags();
    // fmt::print("{0}\n", cpu.get_instruction_pointer());
}
