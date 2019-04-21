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
            // star<
            //     not_at< eolf >,
            //     any
            // >
        >
    {};

    struct ignored :
        sor<
            space,
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

    // A system register (control or debug)
    // As of right now, there are only 4 of each,
    // so 1 digit will suffice.
    struct system_register :
        seq<
            sor<
                TAO_PEGTL_ISTRING("cr"),
                TAO_PEGTL_ISTRING("dr")
            >,
            digit
        >
    {};

    // A reference to a variable/constant
    struct reference : identifier {};

    // A reference to a memory offset
    struct memory_offset : identifier {};

    // The declaration of a named constant from an "eq" directive
    struct constant_declaration : identifier {};

    // The value given to a constant in an "eq" directive
    struct constant_value :  number {};

    // A count for the "zh" and "zw" directives
    struct reserve_count : 
        sor<
            number,
            reference
        >
    {};

    // Immediate values
    // We have different rules for different sizes
    // so we can attach individual actions to them.
    struct immediate_6 : 
        sor<
            number,
            reference
        >
    {};

    struct immediate_9 : 
        sor<
            number,
            reference
        >
    {};

    struct memory_6 :
        sor<
            number,
            reference
        >
    {};

    struct memory_12 :
        sor<
            number,
            reference
        >
    {};

    struct memory_offset_6 :
        sor<
            number,
            memory_offset
        >
    {};

    struct memory_offset_12 :
        sor<
            number,
            memory_offset
        >
    {};
}}

#endif /* TRIREME_ASSEMBLER_TOKENS_HPP */