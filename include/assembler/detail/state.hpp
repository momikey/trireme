#ifndef TRIREME_ASSEMBLER_STATE_HPP
#define TRIREME_ASSEMBLER_STATE_HPP

#include <queue>
#include <map>
#include <string>

namespace ternary { namespace assembler {

    struct state
    {
        int converted;
        int o,m,t,x,y,z;
        std::string op;
        std::queue<int> operands {};
        int instruction_pointer;

        std::map<std::string, int> symbol_table {};
    };

}}

#endif /* TRIREME_ASSEMBLER_STATE_HPP */