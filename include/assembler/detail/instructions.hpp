#ifndef TRIREME_ASSEMBLER_INSTRUCTIONS_HPP
#define TRIREME_ASSEMBLER_INSTRUCTIONS_HPP

#include <tao/pegtl.hpp>

#include "tokens.hpp"

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

    struct in_pfc :
        TAO_PEGTL_KEYWORD("pfc")
    {};
    
    struct in_pfs :
        TAO_PEGTL_KEYWORD("pfs")
    {};
    
    struct in_pfd :
        TAO_PEGTL_KEYWORD("pfd")
    {};
    
    struct in_pfa :
        TAO_PEGTL_KEYWORD("pfa")
    {};
    
    struct in_pfb :
        TAO_PEGTL_KEYWORD("pfb")
    {};
    
    struct in_pft :
        TAO_PEGTL_KEYWORD("pft")
    {};
    
    struct in_pfi :
        TAO_PEGTL_KEYWORD("pfi")
    {};
    
    struct in_pfp :
        TAO_PEGTL_KEYWORD("pfp")
    {};
    
    struct in_zfc :
        TAO_PEGTL_KEYWORD("zfc")
    {};
    
    struct in_zfs :
        TAO_PEGTL_KEYWORD("zfs")
    {};
    
    struct in_zfd :
        TAO_PEGTL_KEYWORD("zfd")
    {};
    
    struct in_zfa :
        TAO_PEGTL_KEYWORD("zfa")
    {};
    
    struct in_zfb :
        TAO_PEGTL_KEYWORD("zfb")
    {};
    
    struct in_zft :
        TAO_PEGTL_KEYWORD("zft")
    {};
    
    struct in_zfi :
        TAO_PEGTL_KEYWORD("zfi")
    {};
    
    struct in_zfp :
        TAO_PEGTL_KEYWORD("zfp")
    {};

    struct in_nfc :
        TAO_PEGTL_KEYWORD("nfc")
    {};

    struct in_nfs :
        TAO_PEGTL_KEYWORD("nfs")
    {};
    
    struct in_nfd :
        TAO_PEGTL_KEYWORD("nfd")
    {};
    
    struct in_nfa :
        TAO_PEGTL_KEYWORD("nfa")
    {};
    
    struct in_nfb :
        TAO_PEGTL_KEYWORD("nfb")
    {};
    
    struct in_nft :
        TAO_PEGTL_KEYWORD("nft")
    {};
    
    struct in_nfi :
        TAO_PEGTL_KEYWORD("nfi")
    {};
    
    struct in_nfp :
        TAO_PEGTL_KEYWORD("nfp")
    {};

    struct in_spc :
        TAO_PEGTL_KEYWORD("spc")
    {};
    
    struct in_sps :
        TAO_PEGTL_KEYWORD("sps")
    {};
    
    struct in_spd :
        TAO_PEGTL_KEYWORD("spd")
    {};
    
    struct in_spa :
        TAO_PEGTL_KEYWORD("spa")
    {};
    
    struct in_spb :
        TAO_PEGTL_KEYWORD("spb")
    {};
    
    struct in_spt :
        TAO_PEGTL_KEYWORD("spt")
    {};
    
    struct in_spi :
        TAO_PEGTL_KEYWORD("spi")
    {};
    
    struct in_spp :
        TAO_PEGTL_KEYWORD("spp")
    {};
    
    struct in_szc :
        TAO_PEGTL_KEYWORD("szc")
    {};
    
    struct in_szs :
        TAO_PEGTL_KEYWORD("szs")
    {};
    
    struct in_szd :
        TAO_PEGTL_KEYWORD("szd")
    {};
    
    struct in_sza :
        TAO_PEGTL_KEYWORD("sza")
    {};
    
    struct in_szb :
        TAO_PEGTL_KEYWORD("szb")
    {};
    
    struct in_szt :
        TAO_PEGTL_KEYWORD("szt")
    {};
    
    struct in_szi :
        TAO_PEGTL_KEYWORD("szi")
    {};
    
    struct in_szp :
        TAO_PEGTL_KEYWORD("szp")
    {};

    struct in_snc :
        TAO_PEGTL_KEYWORD("snc")
    {};

    struct in_sns :
        TAO_PEGTL_KEYWORD("sns")
    {};
    
    struct in_snd :
        TAO_PEGTL_KEYWORD("snd")
    {};
    
    struct in_sna :
        TAO_PEGTL_KEYWORD("sna")
    {};
    
    struct in_snb :
        TAO_PEGTL_KEYWORD("snb")
    {};
    
    struct in_snt :
        TAO_PEGTL_KEYWORD("snt")
    {};
    
    struct in_sni :
        TAO_PEGTL_KEYWORD("sni")
    {};
    
    struct in_snp :
        TAO_PEGTL_KEYWORD("snp")
    {};

    struct in_inc :
        TAO_PEGTL_KEYWORD("inc")
    {};
    
    struct in_dec :
        TAO_PEGTL_KEYWORD("dec")
    {};
    
    struct in_neg :
        TAO_PEGTL_KEYWORD("neg")
    {};
    
    struct in_clr :
        TAO_PEGTL_KEYWORD("clr")
    {};
    
    struct in_srp :
        TAO_PEGTL_KEYWORD("srp")
    {};
    
    struct in_srn :
        TAO_PEGTL_KEYWORD("srn")
    {};
    
    struct in_srz :
        TAO_PEGTL_KEYWORD("srz")
    {};
    
    struct in_psh :
        TAO_PEGTL_KEYWORD("psh")
    {};
    
    struct in_pop :
        TAO_PEGTL_KEYWORD("pop")
    {};
    
    struct in_sti :
        TAO_PEGTL_KEYWORD("sti")
    {};
    
    struct in_pti :
        TAO_PEGTL_KEYWORD("pti")
    {};
    
    struct in_nti :
        TAO_PEGTL_KEYWORD("nti")
    {};
    
    struct in_not :
        TAO_PEGTL_KEYWORD("not")
    {};

    struct flag_positive :
        sor<
            in_pfc,
            in_pfs,
            in_pfd,
            in_pfa,
            in_pfb,
            in_pft,
            in_pfi,
            in_pfp
        >
    {};

    struct flag_zero :
        sor<
            in_zfc,
            in_zfs,
            in_zfd,
            in_zfa,
            in_zfb,
            in_zft,
            in_zfi,
            in_zfp
        >
    {};

    struct flag_negative :
        sor<
            in_nfc,
            in_nfs,
            in_nfd,
            in_nfa,
            in_nfb,
            in_nft,
            in_nfi,
            in_nfp
        >
    {};

    struct skip_positive :
        sor<
            in_spc,
            in_sps,
            in_spd,
            in_spa,
            in_spb,
            in_spt,
            in_spi,
            in_spp
        >
    {};

    struct skip_zero :
        sor<
            in_szc,
            in_szs,
            in_szd,
            in_sza,
            in_szb,
            in_szt,
            in_szi,
            in_szp
        >
    {};

    struct skip_negative :
        sor<
            in_snc,
            in_sns,
            in_snd,
            in_sna,
            in_snb,
            in_snt,
            in_sni,
            in_snp
        >
    {};
    
    struct no_operand :
        sor<
            in_ret,
            in_srt,
            in_brk,
            in_nop,
            in_und,
            flag_positive,
            flag_zero,
            flag_negative,
            skip_positive,
            skip_zero,
            skip_negative
        >
    {};

    struct logical_single_t :
        seq<
            sor<
                in_inc
            >,
            star< blank >,
            cpu_register
        >
    {};

    struct logical_single_y :
        seq<
            sor<
                in_clr,
                in_srp,
                in_srn,
                in_srz
            >,
            star< blank >,
            cpu_register
        >
    {};

    struct logical_single_z :
        seq<
            sor<
                in_psh,
                in_pop,
                in_sti,
                in_pti,
                in_nti,
                in_not,
                in_neg
            >,
            star< blank >,
            cpu_register
        >
    {};

    struct logical_single :
        sor<
            logical_single_t,
            logical_single_y,
            logical_single_z
        >
    {};

    struct instruction :
        sor<
            no_operand,
            logical_single
        >
    {};
}}

#endif /* TRIREME_ASSEMBLER_INSTRUCTIONS_HPP */