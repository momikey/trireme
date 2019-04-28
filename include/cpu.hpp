#ifndef TRIREME_CPU_HPP
#define TRIREME_CPU_HPP

#include <array>
#include <string>
#include <iostream>

#include "registers.hpp"
#include "memory.hpp"
#include "io.hpp"
#include "flags.hpp"

#include "word.hpp"
#include "hexad.hpp"
#include "ternary_math.hpp"

namespace ternary
{
    class Cpu
    {
        public:

        Cpu() = default;

        Word get_register(int reg) const { return registers.get(reg); }
        Hexad get_memory(int address) const { return memory.get(address); }
        Word get_memory_word(int address) const
            { return { memory.get(address+2), memory.get(address+1), memory.get(address) }; }

        void debug_print_flags() const { std::clog << flag_register.to_string() << '\n'; };

        private:
        static constexpr auto control_register_count = 4;
        static constexpr auto debug_register_count = 4;

        Registers registers;
        BasicMemory memory;
        FlagRegister flag_register;
        Io io;
        Word instruction_pointer;

        // These are special, so handle them separately
        std::array<Word, control_register_count> control_regs;
        std::array<Word, debug_register_count> debug_regs;
    };
}

#endif /* TRIREME_CPU_HPP */