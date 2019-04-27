#ifndef TRIREME_CPU_HPP
#define TRIREME_CPU_HPP

#include <array>

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

        private:
        static constexpr auto control_register_count = 4;
        static constexpr auto debug_register_count = 4;

        Registers registers;
        BasicMemory memory;
        Flags flags;
        Io io;

        // These are special, so handle them separately
        std::array<Word, control_register_count> control_regs;
        std::array<Word, debug_register_count> debug_regs;
    };
}

#endif /* TRIREME_CPU_HPP */