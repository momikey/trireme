#ifndef TRIREME_ASSEMBLER_GRAMMAR_HPP
#define TRIREME_ASSEMBLER_GRAMMAR_HPP

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/integer.hpp>

#include "tokens.hpp"
#include "instructions.hpp"
#include "directives.hpp"

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
        seq<
            star< ignored >,
            star< label >,
            sor<
                directive,
                instruction
            >,
            star< ignored >
        >
    {};

    struct grammar :
        seq<
            plus<
                sor<
                    line
                // if_must<
                //     line,
                //     eol
                // >
                >
            >,
            eof
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