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

    struct directive_ad :
        seq<
            TAO_PEGTL_KEYWORD("ad"),
            star< blank >,
            constant_value
        >
    {};

    struct directive :
        sor<
            directive_eq,
            directive_ad
        >
    {};
}}

#endif /* TRIREME_ASSEMBLER_DIRECTIVES_HPP */