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

        const Word value;

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

        const int o;
        const int m;
        const int t;
        const int x;
        const int y;
        const int z;

        int low6() const;
        int low9() const;
        int low12() const;
    };
}

#endif /* TRIREME_OPCODE_HPP */