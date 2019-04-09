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

    struct in_cmp :
        TAO_PEGTL_KEYWORD("cmp")
    {};
    
    struct in_mov :
        TAO_PEGTL_KEYWORD("mov")
    {};
    
    struct in_xch :
        TAO_PEGTL_KEYWORD("xch")
    {};
    
    struct in_bin :
        TAO_PEGTL_KEYWORD("bin")
    {};
    
    struct in_tri :
        TAO_PEGTL_KEYWORD("tri")
    {};
    
    struct in_fdr :
        TAO_PEGTL_KEYWORD("fdr")
    {};
    
    struct in_rdr :
        TAO_PEGTL_KEYWORD("rdr")
    {};
    
    struct in_brr :
        TAO_PEGTL_KEYWORD("brr")
    {};
    
    struct in_caa :
        TAO_PEGTL_KEYWORD("caa")
    {};
    
    struct in_sys :
        TAO_PEGTL_KEYWORD("sys")
    {};
    
    struct in_int :
        TAO_PEGTL_KEYWORD("int")
    {};
    
    struct in_inb :
        TAO_PEGTL_KEYWORD("inb")
    {};
    
    struct in_out :
        TAO_PEGTL_KEYWORD("out")
    {};
    
    struct in_oub :
        TAO_PEGTL_KEYWORD("oub")
    {};
    
    struct in_cmi :
        TAO_PEGTL_KEYWORD("cmi")
    {};
    
    struct in_shl :
        TAO_PEGTL_KEYWORD("shl")
    {};
    
    struct in_shr :
        TAO_PEGTL_KEYWORD("shr")
    {};
    
    struct in_rol :
        TAO_PEGTL_KEYWORD("rol")
    {};
    
    struct in_ror :
        TAO_PEGTL_KEYWORD("ror")
    {};
    
    struct in_rcl :
        TAO_PEGTL_KEYWORD("rcl")
    {};
    
    struct in_rcr :
        TAO_PEGTL_KEYWORD("rcr")
    {};
    
    struct in_bri :
        TAO_PEGTL_KEYWORD("bri")
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

    struct logical_double :
        seq<
            sor<
                in_cmp,
                in_bin,
                in_tri,
                in_fdr,
                in_rdr
            >,
            star< blank >,
            register_pair
        >
    {};

    struct logical_immediate :
        seq<
            sor<
                in_cmi,
                in_shl,
                in_shr,
                in_rol,
                in_ror,
                in_rcl,
                in_rcr
            >,
            star< blank >,
            cpu_register,
            star< blank >,
            one< ','>,
            star< blank >,
            immediate_6
        >
    {};

    struct move_basic :
        seq<
            sor<
                in_mov,
                in_xch
            >,
            star< blank >,
            register_pair
        >
    {};

    struct branch_indirect :
        seq<
            sor<
                in_brr,
                in_caa
            >,
            star< blank >,
            cpu_register
        >
    {};

    struct branch_immediate :
        seq<
            in_bri,
            star< blank >,
            cpu_register,
            star< blank >,
            one< ','>,
            star< blank >,
            immediate_6
        >
    {};
    
    struct branch_vector :
        seq<
            in_sys,
            star< blank >,
            system_call_vector
        >
    {};

    struct input :
        seq<
            sor<
                in_int,
                in_inb
            >,
            star< blank >,
            io_address,
            star< blank >,
            one< ',' >,
            star< blank >,
            cpu_register
        >
    {};

    struct output :
        seq<
            sor<
                in_out,
                in_oub
            >,
            star< blank >,
            cpu_register,
            star< blank >,
            one< ',' >,
            star< blank >,
            io_address
        >
    {};

    struct instruction :
        sor<
            no_operand,
            logical_single,
            logical_double,
            logical_immediate,
            move_basic,
            branch_indirect,
            branch_immediate,
            branch_vector,
            input,
            output
        >
    {};
}}

#endif /* TRIREME_ASSEMBLER_INSTRUCTIONS_HPP */