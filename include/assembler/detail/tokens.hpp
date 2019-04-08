#ifndef TRIREME_ASSEMBLER_TOKENS_HPP
#define TRIREME_ASSEMBLER_TOKENS_HPP

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/integer.hpp>

namespace ternary { namespace assembler {
    using namespace tao::pegtl;

    // Comments are as in most assemblers: semicolon to EOL
    struct comment : 
        seq<
            one< ';' >,
            until< eolf >
        >
    {};

    struct ignored :
        sor<
            blank,
            comment
        >
    {};

    // Decimal integers can use a prepackaged rule/action
    struct integer : tao::pegtl::integer::signed_rule
    {};

    // A single balanced base-27 "digit"
    struct balanced_digit :
        ranges<
            'A', 'M',
            'n', 'z',
            '0'
        >
    {};

    // Balanced base-27 notation, as seen elsewhere
    struct balanced_integer :
        rep_min_max<1, 6,
            balanced_digit
        >
    {};

    // A balanced integer string, introduced by %
    struct balanced_notation :
        seq<
            one< '%' >,
            balanced_integer
        >
    {};

    // Either kind of number
    struct number :
        sor<
            integer,
            balanced_notation
        >
    {};

    // A single register
    // Note that we can't use the simple name "register" because
    // it's a reserved word.
    struct cpu_register :
        seq<
            one< 'r', 'R' >,
            balanced_digit
        >
    {};

}}

#endif /* TRIREME_ASSEMBLER_TOKENS_HPP */