#ifndef TRIREME_INSTRUCTION_ACTIONS_HPP
#define TRIREME_INSTRUCTION_ACTIONS_HPP

#include <tao/pegtl.hpp>

#include <iostream>

#include "instructions.hpp"

namespace ternary { namespace assembler {
    using namespace tao::pegtl;

    // Actions for assembler instructions
    template<>
    struct action<in_ret>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // RET: +0+ ---
            s.o = 10;
            s.m = -13;
            s.t = s.x = s.y = s.z = 0;
            s.op = "ret";
        }
    };

    template<>
    struct action<in_srt>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // SRT: 000 ---
            s.m = -13;
            s.o = s.t = s.x = s.y = s.z = 0;
            s.op = "srt";
        }
    };

    template<>
    struct action<in_brk>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // BRK: 000 --0
            s.m = -12;
            s.o = s.t = s.x = s.y = s.z = 0;
            s.op = "brk";
        }
    };

    template<>
    struct action<in_nop>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // NOP = 000 --+
            s.m = -11;
            s.o = s.t = s.x = s.y = s.z = 0;
            s.op = "nop";
        }
    };

    template<>
    struct action<in_und>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // UND = 000 000
            s.o = s.m = s.t = s.x = s.y = s.z = 0;
            s.op = "und";
        }
    };

    // TODO: Testing
    template<>
    struct action<instruction>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            std::cout
                << s.op << '\t'
                << s.o << '\t'
                << s.m << '\t'
                << s.t << '\t'
                << s.x << '\t'
                << s.y << '\t'
                << s.z << '\n';
        }
    };
}}

#endif /* TRIREME_INSTRUCTION_ACTIONS_HPP */