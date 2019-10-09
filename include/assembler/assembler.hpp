#ifndef TRIREME_ASSEMBLER_ASSEMBLER_HPP
#define TRIREME_ASSEMBLER_ASSEMBLER_HPP

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/tracer.hpp>

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <iterator>

#include "../hexad.hpp"
#include "detail/grammar.hpp"
#include "detail/actions.hpp"
#include "detail/instruction_actions.hpp"
#include "detail/firstpass_actions.hpp"
#include "detail/state.hpp"

namespace ternary { namespace assembler {
    class Assembler
    {
        public:
        using data_map = std::map<int, Hexad>;

        data_map assemble_file(std::string filename);
        data_map assemble_line(std::string line, int origin = 0);
        data_map assemble_string(std::string s);

        private:
        std::size_t analyze() { return tao::pegtl::analyze<grammar>(); }

        template<typename Input, typename State>
        bool first_pass(Input& in, State&& st) { return tao::pegtl::parse<grammar, firstpass>(in, st); }

        template<typename Input, typename State>
        bool assemble(Input& in, State&& st)
        {
            st.instruction_pointer = -1;
            st.operands = decltype(st.operands) {};

            return tao::pegtl::parse<grammar, action>(in, st);
        }

        template<typename State>
        bool assemble_line(const std::string& in, State&& st, int origin)
        {
            using line_and_eof = tao::pegtl::seq<line, tao::pegtl::eolf>;

            tao::pegtl::string_input<> in_fp(in, "immediate");

            st.instruction_pointer = origin;
            st.operands = decltype(st.operands) {};
            auto first_pass_result { tao::pegtl::parse<line_and_eof, firstpass>(in_fp, st) };

            if (first_pass_result)
            {
                st.instruction_pointer = origin;
                st.operands = decltype(st.operands) {};

            }

            tao::pegtl::string_input<> input(in, "immediate");
            return tao::pegtl::parse<line_and_eof, action>(input, st);
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