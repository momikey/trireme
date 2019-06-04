#ifndef TRIREME_CPU_HPP
#define TRIREME_CPU_HPP

#include <array>
#include <string>
#include <iostream>
#include <cassert>
#include <functional>
#include <algorithm>
#include <map>

#include "registers.hpp"
#include "memory.hpp"
#include "io.hpp"
#include "flags.hpp"
#include "opcode.hpp"
#include "debug_io.hpp"
#include "exceptions.hpp"

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

        Cpu();

        // General "control" methods

        void reset();
        void load(std::map<int, Hexad> data);

        // Getters for various parts of the simulator

        Word get_register(int reg) const { return registers.get(reg); }
        Hexad get_memory(int address) const { return memory.get(address); }
        Word get_memory_word(int address) const
            { return { memory.get_word(address) }; }
        Word get_instruction_pointer() const { return instruction_pointer; }

        // Methods for debugging - these give direct access to memory, registers, etc.
        
        void debug_print_flags() const { std::clog << flag_register.to_string() << '\n'; }
        void debug_set_flag(flags f, const int value) { flag_register.set_flag(f, value); }
        void debug_decode_instruction(Opcode& op) { decode_major(op); }
        void debug_set_memory(int addr, int value) { memory.set(addr, value); }
        void debug_set_register(int reg, int value) { registers.set(reg, value); }

        private:
        static constexpr auto control_register_count = 4;
        static constexpr auto debug_register_count = 4;
        static constexpr auto triad = pow3(3);

        Registers registers;
        BasicMemory memory;
        FlagRegister flag_register;
        Io io;
        DebugIo debug_io;
        Word instruction_pointer;

        // These are special, so handle them separately

        std::array<Word, control_register_count+1> control_regs;
        std::array<Word, debug_register_count> debug_regs;

        private:
        using unary_function = std::function<Word(const Word&)>;
        using binary_function = std::function<Word(const Word&, const Word&)>;

        static constexpr auto debug_io_base = 243;

        // Internal methods begin here

        // Instruction decoding

        // Primary instruction decoder
        void decode_major(Opcode&);

        // Decoding for system instructions (O field = 0)
        void decode_minor_system(Opcode&);

        // Decoding for tritwise instructions (O field = 1)
        void decode_minor_tritwise(Opcode&);

        // Decoding for complex arithmetic/conversion instructions (O field = 2)
        void decode_minor_complex(Opcode&);

        // Decoding for arithmetic instructions (O field = 4)
        void decode_minor_arithmetic(Opcode&);

        // Decoding for register instructions (O field = 8)
        void decode_minor_register(Opcode&);

        // Decoding for branch/skip insstructions (O field = 10)
        void decode_minor_branch(Opcode&);

        // Decoding for I/O instructions (O field = -8)
        void decode_minor_io(Opcode&);

        // Decoding for indirect memory instructions (O field = -10)
        void decode_minor_indirect(Opcode&);

        // Decoding for the tertiary operations with O field = 8 and M field = 1
        void decode_tertiary_HA(Opcode&);

        // Instructions
        // These are not 1-to-1 with processor instructions.
        // Instead, we merge those with similar functions and layouts.
        void load_register_memory(const int reg, const int addr, hexad_select type);
        void load_register_immediate(const int reg, const int value, hexad_select type);
        void load_register_indirect(const int addrreg, const int destreg, hexad_select type);
        void load_register_indexed(const int destreg, const int addr);
        void store_register_memory(const int reg, const int addr, hexad_select type);
        void store_register_indirect(const int srcreg, const int addrreg, hexad_select type);
        void store_register_indexed(const int srcreg, const int addr);
        void push_register(const int reg);
        void pop_register(const int reg);

        void add_subtract_register(const int srcreg1, const int srcreg2, const int destreg, const bool subtract);
        void add_subtract_carry(const int srcreg1, const int srcreg2, const int destreg, const bool subtract);
        void add_subtract_immediate(const int srcreg, const int destreg, const int immediate, const bool subtract);
        void multiply_register(const int srcreg1, const int srcreg2, const int destreg);
        void multiply_immediate(const int srcreg, const int destreg, const int immediate);
        void divide_register(const int srcreg1, const int srcreg2, const int destreg);
        void divide_immediate(const int srcreg, const int destreg, const int immediate);

        void register_conversion(const int srcreg, const int destreg, unary_function fun);

        void invert_register(const int reg, unary_function fun);
        void logical_register(const int srcreg1, const int srcreg2, const int destreg, binary_function fun);

        void compare_register(const int lreg, const int rreg);
        void compare_immediate(const int reg, const int immediate);

        void shift_register(const int reg, const int places, bool right);
        void rotate_register(const int reg, const int places, bool right);
        void rotate_register_carry(const int reg, const int places, bool right);

        void set_register(const int reg, const int value);
        void move_register(const int srcreg, const int destreg);
        void exchange_registers(const int lreg, const int rreg);
        void set_flag_to_value(flags f, const int target);

        void branch_absolute(const int address);
        void branch_register(const int reg, const int disp, bool subroutine);
        void branch_relative(const int disp);
        void branch_on_flag(const int addr, const int flag, const int target);
        void branch_ternary(flags f, const int preg, const int zreg, const int nreg);
        void branch_call(const int addr, bool relative);
        void branch_return();

        void system_call(const int vec);
        void system_return();
        void system_breakpoint();
        void system_load_register(const int userreg, const int sysreg);
        void system_store_register(const int userreg, const int sysreg);

        void io_read(const int reg, const int port, bool binary);
        void io_write(const int reg, const int port, bool binary);

        void undefined_opcode(Opcode& op);

        // Helpers to construct values from 3-trit "triads"
        int value_6(int x, int y) { return x* pow3(3) + y; }
        int value_9(int x, int y, int z) { return x * pow3(6) + y * pow3(3) + z; }
        int value_12(int x, int y, int z, int w) { return x + pow3(9) + y * pow3(6) + z * pow3(3) + w; }

        // Memory read/write to handle absolute vs. pointer-based
        Hexad read_memory(const int ad);
        Word read_memory_word(const int ad);
        void write_memory(const int ad, const int val);
        void write_memory_word(const int ad, const int val);

        Hexad read_memory(const Word& ad) { return read_memory(ad.value()); }
        Word read_memory_word(const Word& ad) { return read_memory_word(ad.value()); }
        void write_memory(const Word& ad, const int val) { return write_memory(ad.value(), val); }
        void write_memory(const Word& ad, const Hexad& val) { return write_memory(ad.value(), val.get()); }
        void write_memory_word(const Word& ad, const Word& val) { return write_memory_word(ad.value(), val.value()); }
    };
}

#endif /* TRIREME_CPU_HPP */