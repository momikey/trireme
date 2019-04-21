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

    struct directive_dh :
        seq<
            TAO_PEGTL_KEYWORD("dh"),
            star< blank >,
            list<
                sor<
                    constant_value,
                    reference
                >,
                one< ',' >,
                blank
            >
        >
    {};

    struct directive_dw :
        seq<
            TAO_PEGTL_KEYWORD("dw"),
            star< blank >,
            list<
                sor<
                    constant_value,
                    reference
                >,
                one< ',' >,
                blank
            >
        >
    {};

    struct directive_zh :
        seq<
            TAO_PEGTL_KEYWORD("zh"),
            star< blank >,
            reserve_count
        >
    {};

    struct directive_zw :
        seq<
            TAO_PEGTL_KEYWORD("zw"),
            star< blank >,
            reserve_count
        >
    {};
    struct directive :
        sor<
            directive_eq,
            directive_ad,
            directive_dh,
            directive_dw,
            directive_zh,
            directive_zw
        >
    {};
}}

#endif /* TRIREME_ASSEMBLER_DIRECTIVES_HPP */