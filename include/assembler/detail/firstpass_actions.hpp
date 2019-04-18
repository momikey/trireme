#ifndef TRIREME_FIRSTPASS_ACTIONS_HPP
#define TRIREME_FIRSTPASS_ACTIONS_HPP

#include <tao/pegtl.hpp>

#include <string>

#include "grammar.hpp"

namespace ternary { namespace assembler {
    using namespace tao::pegtl;

    // Do-nothing rule, as in the main pass
    template<typename Rule>
    struct firstpass : nothing<Rule> {};

    template<>
    struct firstpass<instruction>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // All Trireme instructions are 1 word, or 3 hexads
            s.instruction_pointer += 3;
        }
    };

    template<>
    struct firstpass<label>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            std::string tok { in.begin(), in.end() };
            auto sym { tok.substr(0, tok.find(':')) };
            s.symbol_table[sym] = s.instruction_pointer;
        }
    };
}}

#endif /* TRIREME_FIRSTPASS_ACTIONS_HPP */