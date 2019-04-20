#ifndef TRIREME_FIRSTPASS_ACTIONS_HPP
#define TRIREME_FIRSTPASS_ACTIONS_HPP

#include <tao/pegtl.hpp>

#include <iostream>
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

    template<>
    struct firstpass<constant_declaration>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            std::string sym { in.begin(), in.end() };
            s.working_symbol = sym;
        }
    };

    template<>
    struct firstpass<directive_eq>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            auto value { s.operands.front() };
            s.operands.pop();

            s.symbol_table[s.working_symbol] = value;
            s.working_symbol = "";
        }
    };

    template<>
    struct firstpass<constant_value>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.operands.push(s.converted);
        }
    };

    // These are the same in 1st & 2nd pass
    template<>
    struct firstpass<integer> : public tao::pegtl::integer::signed_action
    {};

    template<>
    struct firstpass<balanced_integer>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.converted = string_to_value(in.string());
        }
    };
}}

#endif /* TRIREME_FIRSTPASS_ACTIONS_HPP */