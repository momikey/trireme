#ifndef TRIREME_ASSEMBLER_GRAMMAR_HPP
#define TRIREME_ASSEMBLER_GRAMMAR_HPP

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/integer.hpp>

#include "tokens.hpp"
#include "instructions.hpp"

namespace ternary { namespace assembler
{
    using namespace tao::pegtl;


    struct operand :
        sor<
            number,
            cpu_register,
            instruction // Just for testing
        >
    {};

    struct label_name: identifier {};

    struct label :
        seq<
            label_name,
            one< ':'>,
            star<
                space
            >
        >
    {};

    struct line :
        pad<
            seq<
                opt<
                    label
                >,
                must<
                    operand
                >
            >,
            blank
        >
    {};

    struct grammar :
        plus<
            if_must<
                instruction,
                sor<
                    eolf,
                    ignored
                >
            >
        >
        // until< eof,
        //     plus<
        //         pad_opt<
        //             line,
        //             eol
        //         >
        //     >
        // >
    {};
}}

#endif /* TRIREME_ASSEMBLER_GRAMMAR_H */