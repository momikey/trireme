#ifndef TRIREME_OPCODE_HPP
#define TRIREME_OPCODE_HPP

#include "word.hpp"
#include "ternary_math.hpp"

namespace ternary
{
    struct Opcode
    {
        // Opcodes need no default constructor
        Opcode() = delete;

        Opcode(const Word& w);
        Opcode(int n): Opcode(Word{n}) {}

        Word get() const { return opcode_; }

        int o_field() const { return o_; }
        int m_field() const { return m_; }
        int t_field() const { return t_; }
        int x_field() const { return x_; }
        int y_field() const { return y_; }
        int z_field() const { return z_; }

        int low6() const;
        int low9() const;
        int low12() const;

        private:
        // The opcode itself
        Word opcode_;

        // Fields in the opcode
        // Different operations can have different fields,
        // so we use a generic naming scheme:
        //
        // 18----15----12-----9-----6-----3-----0
        //  |  O  |  M  |  T  |  X  |  Y  |  Z  |
        // --------------------------------------
        //
        // Where:
        // * O - major operation code
        // * M - minor operation code or operating register
        // * T - tertiary operation code or operating register
        // * X,Y,Z - register, memory, or immediate values
        //
        // We store these as `int`s for convenience.

        int o_;
        int m_;
        int t_;
        int x_;
        int y_;
        int z_;
    };
}

#endif /* TRIREME_OPCODE_HPP */