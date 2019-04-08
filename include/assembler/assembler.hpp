#ifndef TRIREME_ASSEMBLER_ASSEMBLER_HPP
#define TRIREME_ASSEMBLER_ASSEMBLER_HPP

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/tracer.hpp>

#include "detail/grammar.hpp"
#include "detail/actions.hpp"
#include "detail/instruction_actions.hpp"

namespace ternary { namespace assembler {
    std::size_t analyze()
    {
        return tao::pegtl::analyze<grammar>();
    }

    template<typename Input, typename State>
    bool assemble(Input& in, State&& st)
    {
        return tao::pegtl::parse<
            grammar,
            action
            // ,tracer
            >
            (in, st);
    }

    int dummy();
}}

#endif /* TRIREME_ASSEMBLER_ASSEMBLER_HPP */