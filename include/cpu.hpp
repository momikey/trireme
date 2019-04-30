#ifndef TRIREME_CPU_HPP
#define TRIREME_CPU_HPP

#include <array>
#include <string>
#include <iostream>
#include <cassert>

#include "registers.hpp"
#include "memory.hpp"
#include "io.hpp"
#include "flags.hpp"
#include "opcode.hpp"

#include "word.hpp"
#include "hexad.hpp"
#include "ternary_math.hpp"

namespace ternary
{
    // Hexad selector for addressing
    enum class hexad_select
    {
        low,
        middle,
        high,
        full_word
    };

    class Cpu
    {
        public:

        Cpu() = default;

        Word get_register(int reg) const { return registers.get(reg); }
        Hexad get_memory(int address) const { return memory.get(address); }
        Word get_memory_word(int address) const
            { return { memory.get(address+2), memory.get(address+1), memory.get(address) }; }

        void debug_print_flags() const { std::clog << flag_register.to_string() << '\n'; }
        void debug_decode_instruction(Opcode& op) { decode_major(op); }
        void debug_set_memory(int addr, int value) { memory.set(addr, value); }

        private:
        static constexpr auto control_register_count = 4;
        static constexpr auto debug_register_count = 4;
        static constexpr auto triad = pow3(3);

        Registers registers;
        BasicMemory memory;
        FlagRegister flag_register;
        Io io;
        Word instruction_pointer;

        // These are special, so handle them separately
        std::array<Word, control_register_count> control_regs;
        std::array<Word, debug_register_count> debug_regs;

        // Internal methods begin here

        // Instruction decoding
        void decode_major(Opcode&);

        // Instructions
        // These are not 1-to-1 with processor instructions.
        // Instead, we merge those with similar functions and layouts.
        void load_register_memory(const int reg, const int addr, hexad_select type);
        void load_register_immediate(const int reg, const int value, hexad_select type);
        void store_register_memory(const int reg, const int addr, hexad_select type);

        // Helpers to construct values from 3-trit "triads"
        int value_6(int x, int y) { return x* pow3(3) + y; }
        int value_9(int x, int y, int z) { return x * pow3(6) + y * pow3(3) + z; }
        int value_12(int x, int y, int z, int w) { return x + pow3(9) + y * pow3(6) + z * pow3(3) + w; }

    };
}

#endif /* TRIREME_CPU_HPP */