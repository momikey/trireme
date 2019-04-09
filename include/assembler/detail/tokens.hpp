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

    // A pair of registers
    struct register_pair :
        seq<
            cpu_register,
            star< blank >,
            one< ',' >,
            star< blank >,
            cpu_register
        >
    {};

    // A system-call vector
    struct system_call_vector :
        seq<
            one< '#' >,
            number
        >
    {};

    // An I/O address
    struct io_address :
        seq<
            one< '@' >,
            number
        >
    {};

    // Immediate values
    // We have different rules for different sizes
    // so we can attach individual actions to them.
    struct immediate_6 : number {};

    struct immediate_9 : number {};

    struct memory_12 : number {};
}}

#endif /* TRIREME_ASSEMBLER_TOKENS_HPP */