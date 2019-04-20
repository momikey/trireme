#ifndef TRIREME_ASSEMBLER_ACTIONS_HPP
#define TRIREME_ASSEMBLER_ACTIONS_HPP

#include <tao/pegtl.hpp>

#include <iostream>
#include <string>

#include "grammar.hpp"
#include "../../convert.hpp"
#include "../../ternary_math.hpp"

namespace ternary { namespace assembler {
    using namespace tao::pegtl;

    constexpr auto io_width = 9;
    constexpr auto syscall_width = 6;

    // A do-nothing action we can use as a base
    template<typename Rule>
    struct action : nothing<Rule> {};

    template<>
    struct action<integer> : public tao::pegtl::integer::signed_action
    {};

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
    struct action<immediate_6>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.operands.push(low_trits(s.converted, 6));
        }
    };

    template<>
    struct action<immediate_9>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.operands.push(low_trits(s.converted, 9));
        }
    };

    // This is the same for now
    template<>
    struct action<memory_6> : action<immediate_6> {};

    template<>
    struct action<memory_offset_6> : action<memory_6> {};

    template<>
    struct action<memory_12>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.operands.push(low_trits(s.converted, 12));
        }
    };

    template<>
    struct action<memory_offset_12> : action<memory_12> {};
    
    template<>
    struct action<cpu_register>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            std::string regnum { in.begin()+1, in.end() };
            s.converted = string_to_value(regnum);

            s.operands.push(s.converted);
        }
    };
    
    template<>
    struct action<system_call_vector>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // std::string vecnum { in.begin()+1, in.end() };
            // s.converted = string_to_value(vecnum);

            s.operands.push(low_trits(s.converted, syscall_width));
        }
    };

    template<>
    struct action<io_address>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // std::string ioaddr { in.begin()+1, in.end() };
            // std::clog << "Found: " << ioaddr << '\n';
            // s.converted = string_to_value(ioaddr);

            s.operands.push(low_trits(s.converted, io_width));
        }
    };
    
    template<>
    struct action<system_register>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            std::string regtype { in.begin(), in.begin()+1 };
            std::string regnum { in.begin()+2, in.end() };
            
            s.converted = std::stoi(regnum);

            if (regtype[0] == 'd' || regtype[0] == 'D')
            {
                s.converted = -s.converted;
            }

            s.operands.push(s.converted);
        }
    };

    template<>
    struct action<reference>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            std::string sym { in.begin(), in.end() };
            std::clog << sym << '\t' << s.symbol_table[sym] << '\n';
            s.converted = s.symbol_table[sym];
        }
    };

    template<>
    struct action<memory_offset>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            std::string sym { in.begin(), in.end() };
            std::clog << s.instruction_pointer << '\t' << sym << '\t' << s.symbol_table[sym] << '\n';
            s.converted = s.symbol_table[sym] - s.instruction_pointer;
        }
    };

    // TODO: Testing
    template<>
    struct action<instruction>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            std::clog
                << s.instruction_pointer << '\t'
                << s.op << '\t'
                << s.o << '\t'
                << s.m << '\t'
                << s.t << '\t'
                << s.x << '\t'
                << s.y << '\t'
                << s.z << '\n'
            ;

            // All Trireme instructions are 1 word, or 3 hexads,
            // and these *must* be word-aligned.
            s.instruction_pointer += 3;
        }
    };

    template<>
    struct action<line>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            if (s.instruction_pointer % 3)
            {
                // Instructions must be word-aligned.
                // We fix that here, although it may mess up
                // insert directives.
                s.instruction_pointer += 3 - (s.instruction_pointer % 3);
            }
        }
    };
}}

#endif /* TRIREME_ASSEMBLER_ACTIONS_HPP */