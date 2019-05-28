#ifndef TRIREME_ASSEMBLER_STATE_HPP
#define TRIREME_ASSEMBLER_STATE_HPP

#include <queue>
#include <map>
#include <string>

#include "../../hexad.hpp"

namespace ternary { namespace assembler {

    struct state
    {
        int o,m,t,x,y,z;
        std::string op;
        int instruction_pointer;
        std::queue<int> operands {};

        std::map<std::string, int> symbol_table {};
        std::string working_symbol;

        std::map<int, Hexad> data {};

        // These are PEGTL internal
        int converted;
        std::string unescaped;
    };
}}

#endif /* TRIREME_ASSEMBLER_STATE_HPP */