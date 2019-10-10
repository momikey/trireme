#ifndef TRIREME_ASSEMBLER_INSTRUCTIONS_HPP
#define TRIREME_ASSEMBLER_INSTRUCTIONS_HPP

#include <tao/pegtl.hpp>

#include "tokens.hpp"

namespace ternary { namespace assembler {
    using namespace tao::pegtl;

    // Grammar rules for assembler instructions

    struct in_add :
        TAO_PEGTL_KEYWORD("add")
    {};
    
    struct in_adc :
        TAO_PEGTL_KEYWORD("adc")
    {};
    
    struct in_sub :
        TAO_PEGTL_KEYWORD("sub")
    {};
    
    struct in_sbc :
        TAO_PEGTL_KEYWORD("sbc")
    {};
    
    struct in_mul :
        TAO_PEGTL_KEYWORD("mul")
    {};
    
    struct in_div :
        TAO_PEGTL_KEYWORD("div")
    {};
    
    struct in_min :
        TAO_PEGTL_KEYWORD("min")
    {};
    
    struct in_max :
        TAO_PEGTL_KEYWORD("max")
    {};
    
    struct in_teq :
        TAO_PEGTL_KEYWORD("teq")
    {};
    
    struct in_tem :
        TAO_PEGTL_KEYWORD("tem")
    {};
    
    struct in_and :
        TAO_PEGTL_KEYWORD("and")
    {};
    
    struct in_ort :
        TAO_PEGTL_KEYWORD("ort")
    {};
    
    struct in_pad :
        TAO_PEGTL_KEYWORD("pad")
    {};
    
    struct in_psb :
        TAO_PEGTL_KEYWORD("psb")
    {};
    
    struct in_pmu :
        TAO_PEGTL_KEYWORD("pmu")
    {};
    
    struct in_pdv :
        TAO_PEGTL_KEYWORD("pdv")
    {};

    struct in_adi :
        TAO_PEGTL_KEYWORD("adi")
    {};
    
    struct in_sbi :
        TAO_PEGTL_KEYWORD("sbi")
    {};
    
    struct in_mli :
        TAO_PEGTL_KEYWORD("mli")
    {};
    
    struct in_dvi :
        TAO_PEGTL_KEYWORD("dvi")
    {};
    
    struct in_ldi :
        TAO_PEGTL_KEYWORD("ldi")
    {};
    
    struct in_lil :
        TAO_PEGTL_KEYWORD("lil")
    {};
    
    struct in_lim :
        TAO_PEGTL_KEYWORD("lim")
    {};
    
    struct in_lih :
        TAO_PEGTL_KEYWORD("lih")
    {};
    

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
    
    struct in_bra :
        TAO_PEGTL_KEYWORD("bra")
    {};
    
    struct in_brs :
        TAO_PEGTL_KEYWORD("brs")
    {};
    
    struct in_brl :
        TAO_PEGTL_KEYWORD("brl")
    {};
    
    struct in_cal :
        TAO_PEGTL_KEYWORD("cal")
    {};

    struct in_car :
        TAO_PEGTL_KEYWORD("car")
    {};
    
    struct in_bpc :
        TAO_PEGTL_KEYWORD("bpc")
    {};
    
    struct in_bps :
        TAO_PEGTL_KEYWORD("bps")
    {};
    
    struct in_bpd :
        TAO_PEGTL_KEYWORD("bpd")
    {};
    
    struct in_bpa :
        TAO_PEGTL_KEYWORD("bpa")
    {};
    
    struct in_bpb :
        TAO_PEGTL_KEYWORD("bpb")
    {};
    
    struct in_bpt :
        TAO_PEGTL_KEYWORD("bpt")
    {};
    
    struct in_bpi :
        TAO_PEGTL_KEYWORD("bpi")
    {};
    
    struct in_bpp :
        TAO_PEGTL_KEYWORD("bpp")
    {};
    
    struct in_bzc :
        TAO_PEGTL_KEYWORD("bzc")
    {};
    
    struct in_bzs :
        TAO_PEGTL_KEYWORD("bzs")
    {};
    
    struct in_bzd :
        TAO_PEGTL_KEYWORD("bzd")
    {};
    
    struct in_bza :
        TAO_PEGTL_KEYWORD("bza")
    {};
    
    struct in_bzb :
        TAO_PEGTL_KEYWORD("bzb")
    {};
    
    struct in_bzt :
        TAO_PEGTL_KEYWORD("bzt")
    {};
    
    struct in_bzi :
        TAO_PEGTL_KEYWORD("bzi")
    {};
    
    struct in_bzp :
        TAO_PEGTL_KEYWORD("bzp")
    {};
    
    struct in_bnc :
        TAO_PEGTL_KEYWORD("bnc")
    {};
    
    struct in_bns :
        TAO_PEGTL_KEYWORD("bns")
    {};
    
    struct in_bnd :
        TAO_PEGTL_KEYWORD("bnd")
    {};
    
    struct in_bna :
        TAO_PEGTL_KEYWORD("bna")
    {};
    
    struct in_bnb :
        TAO_PEGTL_KEYWORD("bnb")
    {};
    
    struct in_bnt :
        TAO_PEGTL_KEYWORD("bnt")
    {};
    
    struct in_bni :
        TAO_PEGTL_KEYWORD("bni")
    {};
    
    struct in_bnp :
        TAO_PEGTL_KEYWORD("bnp")
    {};

    struct in_tbc :
        TAO_PEGTL_KEYWORD("tbc")
    {};
    
    struct in_tbs :
        TAO_PEGTL_KEYWORD("tbs")
    {};
    
    struct in_tbd :
        TAO_PEGTL_KEYWORD("tbd")
    {};
    
    struct in_tba :
        TAO_PEGTL_KEYWORD("tba")
    {};
    
    struct in_tbb :
        TAO_PEGTL_KEYWORD("tbb")
    {};
    
    struct in_tbt :
        TAO_PEGTL_KEYWORD("tbt")
    {};
    
    struct in_tbi :
        TAO_PEGTL_KEYWORD("tbi")
    {};
    
    struct in_tbp :
        TAO_PEGTL_KEYWORD("tbp")
    {};
    
    struct in_ldr :
        TAO_PEGTL_KEYWORD("ldr")
    {};
    
    struct in_ldl :
        TAO_PEGTL_KEYWORD("ldl")
    {};
    
    struct in_ldm :
        TAO_PEGTL_KEYWORD("ldm")
    {};
    
    struct in_ldh :
        TAO_PEGTL_KEYWORD("ldh")
    {};
    
    struct in_ldn :
        TAO_PEGTL_KEYWORD("ldn")
    {};
    
    struct in_str :
        TAO_PEGTL_KEYWORD("str")
    {};
    
    struct in_stl :
        TAO_PEGTL_KEYWORD("stl")
    {};
    
    struct in_stm :
        TAO_PEGTL_KEYWORD("stm")
    {};
    
    struct in_sth :
        TAO_PEGTL_KEYWORD("sth")
    {};
    
    struct in_stn :
        TAO_PEGTL_KEYWORD("stn")
    {};
    
    struct in_lda :
        TAO_PEGTL_KEYWORD("lda")
    {};
    
    struct in_lal :
        TAO_PEGTL_KEYWORD("lal")
    {};
    
    struct in_lam :
        TAO_PEGTL_KEYWORD("lam")
    {};
    
    struct in_lah :
        TAO_PEGTL_KEYWORD("lah")
    {};
    
    struct in_sta :
        TAO_PEGTL_KEYWORD("sta")
    {};
    
    struct in_sal :
        TAO_PEGTL_KEYWORD("sal")
    {};
    
    struct in_sam :
        TAO_PEGTL_KEYWORD("sam")
    {};
    
    struct in_sah :
        TAO_PEGTL_KEYWORD("sah")
    {};
    
    struct in_lsr :
        TAO_PEGTL_KEYWORD("lsr")
    {};
    
    struct in_ssr :
        TAO_PEGTL_KEYWORD("ssr")
    {};

    struct in_lad :
        TAO_PEGTL_KEYWORD("lad")
    {};
    
    struct arithmetic_basic_instruction :
        sor<
            in_add,
            in_adc,
            in_sub,
            in_sbc,
            in_mul,
            in_div,
            in_min,
            in_max,
            in_teq,
            in_tem,
            in_and,
            in_ort
        >
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

    struct arithmetic_basic :
        seq<
            arithmetic_basic_instruction,
            star< blank >,
            cpu_register,
            rep_max< 2,
                star< blank >,
                one< ',' >,
                star< blank >,
                cpu_register
            >
        >
    {};

    struct arithmetic_immediate :
        seq<
            sor<
                in_adi,
                in_sbi,
                in_mli,
                in_dvi
            >,
            star< blank >,
            cpu_register,
            opt<
                one< ',' >,
                star< blank >,
                cpu_register
            >,
            one< ',' >,
            star< blank >,
            immediate_6
        >
    {};

    struct arithmetic_inplace :
        seq<
            sor<
                in_pad,
                in_psb,
                in_pmu,
                in_pdv
            >,
            star< blank >,
            cpu_register,
            star< blank >,
            one< ',' >,
            star< blank >,
            immediate_9
        >
    {};

    struct logical_single_t :
        seq<
            sor<
                in_inc,
                in_dec
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

    struct branch_basic :
        seq<
            sor<
                in_bra,
                in_cal
            >,
            star< blank >,
            memory_12
        >
    {};

    struct branch_offset_6 :
        seq<
            in_brs,
            star< blank >,
            memory_offset_6
        >
    {};

    struct branch_offset_12 :
        seq<
            sor<
                in_brl,
                in_bpc,
                in_bps,
                in_bpd,
                in_bpa,
                in_bpb,
                in_bpt,
                in_bpi,
                in_bpp,
                in_bzc,
                in_bzs,
                in_bzd,
                in_bza,
                in_bzb,
                in_bzt,
                in_bzi,
                in_bzp,
                in_bnc,
                in_bns,
                in_bnd,
                in_bna,
                in_bnb,
                in_bnt,
                in_bni,
                in_bnp,
                in_car
            >,
            star< blank >,
            memory_offset_12
        >
    {};

    struct branch_offset :
        sor<
            branch_offset_6,
            branch_offset_12
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
            one< ',' >,
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

    struct branch_ternary :
        seq<
            sor<
                in_tbc,
                in_tbs,
                in_tbd,
                in_tba,
                in_tbb,
                in_tbt,
                in_tbi,
                in_tbp
            >,
            star< blank >,
            cpu_register,
            star< blank >,
            one< ',' >,
            star< blank >,
            cpu_register,
            star< blank >,
            one< ',' >,
            star< blank >,
            cpu_register
        >
    {};

    struct load_basic :
        seq<
            sor<
                in_ldr,
                in_ldl,
                in_ldm,
                in_ldh
            >,
            star< blank >,
            memory_12,
            star< blank >,
            one< ',' >,
            star< blank >,
            cpu_register
        >
    {};

    struct load_immediate :
        seq<
            sor<
                in_ldi,
                in_lil,
                in_lim,
                in_lih
            >,
            star< blank >,
            cpu_register,
            star< blank >,
            one< ',' >,
            star< blank >,
            immediate_6
        >
    {};

    struct load_indexed :
        seq<
            in_ldn,
            star< blank >,
            memory_offset_6,
            star< blank >,
            one< ',' >,
            star< blank >,
            cpu_register
        >
    {};

    struct load_address :
        seq<
            in_lad,
            star< blank >,
            memory_12
        >
    {};

    struct store_basic :
        seq<
            sor<
                in_str,
                in_stl,
                in_stm,
                in_sth
            >,
            star< blank >,
            cpu_register,
            star< blank >,
            one< ',' >,
            star< blank >,
            memory_12
        >
    {};

    struct store_indexed :
        seq<
            in_stn,
            star< blank >,
            cpu_register,
            star< blank >,
            one< ',' >,
            star< blank >,
            memory_offset_6
        >
    {};

    struct load_store_indirect :
        seq<
            sor<
                in_lda,
                in_lal,
                in_lam,
                in_lah,
                in_sta,
                in_sal,
                in_sam,
                in_sah
            >,
            star< blank >,
            cpu_register,
            star< blank >,
            one< ',' >,
            star< blank >,
            cpu_register
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

    struct load_system :
        seq<
            in_lsr,
            star< blank >,
            system_register,
            star< blank >,
            one< ',' >,
            star< blank >,
            cpu_register
        >
    {};

    struct store_system :
        seq<
            in_ssr,
            star< blank >,
            cpu_register,
            star< blank >,
            one< ',' >,
            star< blank >,
            system_register
        >
    {};

    struct instruction :
        sor<
            no_operand,
            arithmetic_basic,
            arithmetic_immediate,
            arithmetic_inplace,
            logical_single,
            logical_double,
            logical_immediate,
            move_basic,
            branch_basic,
            branch_indirect,
            branch_offset,
            branch_immediate,
            branch_vector,
            branch_ternary,
            load_basic,
            load_immediate,
            load_indexed,
            load_address,
            store_basic,
            store_indexed,
            load_store_indirect,
            load_system,
            store_system,
            input,
            output
        >
    {};
}}

#endif /* TRIREME_ASSEMBLER_INSTRUCTIONS_HPP */