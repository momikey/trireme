#ifndef TRIREME_ASSEMBLER_DIRECTIVES_HPP
#define TRIREME_ASSEMBLER_DIRECTIVES_HPP

#include <tao/pegtl.hpp>

#include "tokens.hpp"

namespace ternary { namespace assembler {
    using namespace tao::pegtl;

    struct directive_eq :
        seq<
            TAO_PEGTL_KEYWORD("eq"),
            star< blank >,
            constant_declaration,
            star< blank >,
            one< ',' >,
            star< blank >,
            constant_value
        >
    {};

    struct directive :
        sor<
            directive_eq
        >
    {};
}}

#endif /* TRIREME_ASSEMBLER_DIRECTIVES_HPP */