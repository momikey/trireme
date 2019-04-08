#ifndef TRIREME_ASSEMBLER_ACTIONS_HPP
#define TRIREME_ASSEMBLER_ACTIONS_HPP

#include <tao/pegtl.hpp>

#include <iostream>
#include <string>

#include "grammar.hpp"
#include "convert.hpp"

namespace ternary { namespace assembler {
    using namespace tao::pegtl;

    // A do-nothing action we can use as a base
    template<typename Rule>
    struct action : nothing<Rule> {};

    template<>
    struct action<integer> : public tao::pegtl::integer::signed_action
    {};

    template<>
    struct action<number>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            std::clog << in.string() << '\t' << s.converted << '\n';
        }
    };

    template<>
    struct action<balanced_integer>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.converted = string_to_value(in.string());
        }
    };

    template<>
    struct action<cpu_register>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            std::string regnum { in.begin()+1, in.end() };
            s.converted = string_to_value(regnum);

            s.operands.push(s.converted);

            std::clog << "Register " << regnum << '\n';
        }
    };

    template<>
    struct action<label_name>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            std::clog << in.string() << '\n';
        }
    };
}}

#endif /* TRIREME_ASSEMBLER_ACTIONS_HPP */