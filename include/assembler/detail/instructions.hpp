#ifndef TRIREME_ASSEMBLER_INSTRUCTIONS_HPP
#define TRIREME_ASSEMBLER_INSTRUCTIONS_HPP

#include <tao/pegtl.hpp>

namespace ternary { namespace assembler {
    using namespace tao::pegtl;

    // Grammar rules for assembler instructions

    struct in_ret :
        TAO_PEGTL_KEYWORD("ret")
    {};

    struct in_srt :
        TAO_PEGTL_KEYWORD("srt")
    {};
    
    struct in_brk :
        TAO_PEGTL_KEYWORD("brk")
    {};
    
    struct in_nop :
        TAO_PEGTL_KEYWORD("nop")
    {};
    
    struct in_und :
        TAO_PEGTL_KEYWORD("und")
    {};
    
    struct no_operand:
        sor<
            in_ret,
            in_srt,
            in_brk,
            in_nop,
            in_und  // TODO add skip and flag set
        >
    {};

    struct instruction:
        sor<
            no_operand
        >
    {};
}}

#endif /* TRIREME_ASSEMBLER_INSTRUCTIONS_HPP */