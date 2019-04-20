#ifndef TRIREME_ASSEMBLER_ASSEMBLER_HPP
#define TRIREME_ASSEMBLER_ASSEMBLER_HPP

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/tracer.hpp>

#include <iostream>
#include <string>

#include "detail/grammar.hpp"
#include "detail/actions.hpp"
#include "detail/instruction_actions.hpp"
#include "detail/firstpass_actions.hpp"
#include "detail/state.hpp"

namespace ternary { namespace assembler {
    class Assembler
    {
        public:
        bool assemble_file(std::string filename);
        bool assemble_string(std::string s);

        private:
        std::size_t analyze() { return tao::pegtl::analyze<grammar>(); }

        template<typename Input, typename State>
        bool first_pass(Input& in, State&& st) { return tao::pegtl::parse<grammar, firstpass>(in, st); }

        template<typename Input, typename State>
        bool assemble(Input& in, State&& st)
        {
            st.instruction_pointer = 0;
            st.operands = decltype(st.operands) {};

            return tao::pegtl::parse<grammar, action>(in, st);
        }

        state st_ {};
    };

    template<typename Input, typename State>
    bool first_pass(Input& in, State&& st)
    {
        return tao::pegtl::parse<
            grammar,
            firstpass
            // ,tracer
            >
            (in, st);
    }

    template<typename Input, typename State>
    bool assemble(Input& in, State&& st)
    {
        st.instruction_pointer = 0;
        st.operands = decltype(st.operands) {};

        return tao::pegtl::parse<
            grammar,
            action
            // ,tracer
            >
            (in, st);
    }
}}

#endif /* TRIREME_ASSEMBLER_ASSEMBLER_HPP */