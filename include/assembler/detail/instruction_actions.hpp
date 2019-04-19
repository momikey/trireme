#ifndef TRIREME_INSTRUCTION_ACTIONS_HPP
#define TRIREME_INSTRUCTION_ACTIONS_HPP

#include <tao/pegtl.hpp>

#include <iostream>

#include "instructions.hpp"
#include "../../ternary_math.hpp"

namespace ternary { namespace assembler {
    using namespace tao::pegtl;

    // These are the CPU flags, which are used in certain opcodes
    enum class Flags
    {
        carry = 0,
        sign,
        direction,
        // Flags 3 & 4 unused
        absolute = 5,
        binary,
        trap,
        interrupt,
        protect
        // All flags >9 unused
    };
    
    namespace detail
    {
        // Helper functions for flags, etc.,
        // so we don't have to repeat everything.
        template<Flags f, typename Input, typename State>
        inline void set_flag_positive(const Input& in, State& s)
        {
            // PFf -+0 00+ 000 000 000 fff
            s.o = -6;
            s.m = 1;
            s.z = static_cast<int>(f);
            s.t = s.x = s.y = 0;
        }

        template<Flags f, typename Input, typename State>
        inline void set_flag_zero(const Input& in, State& s)
        {
            // ZFf -+0 000 000 000 000 fff
            s.o = -6;
            s.m = 0;
            s.z = static_cast<int>(f);
            s.t = s.x = s.y = 0;
        }

        template<Flags f, typename Input, typename State>
        inline void set_flag_negative(const Input& in, State& s)
        {
            // NFf -+0 00+ 000 000 000 fff
            s.o = -6;
            s.m = -1;
            s.z = static_cast<int>(f);
            s.t = s.x = s.y = 0;
        }

        template<Flags f, typename Input, typename State>
        inline void skip_flag_positive(const Input& in, State& s)
        {
            // SPf +0+ +++ ... ... ... fff
            s.o = 10;
            s.m = 13;
            s.z = static_cast<int>(f);
            s.t = s.x = s.y = 0;
        }

        template<Flags f, typename Input, typename State>
        inline void skip_flag_zero(const Input& in, State& s)
        {
            // SZf +0+ ++0 ... ... ... fff
            s.o = 10;
            s.m = 12;
            s.z = static_cast<int>(f);
            s.t = s.x = s.y = 0;
        }

        template<Flags f, typename Input, typename State>
        inline void skip_flag_negative(const Input& in, State& s)
        {
            // SNf +0+ +++ ... ... ... fff
            s.o = 10;
            s.m = 11;
            s.z = static_cast<int>(f);
            s.t = s.x = s.y = 0;
        }

        template<Flags f, typename Input, typename State>
        inline void branch_flag_positive(const Input& in, State& s)
        {
            // BPf +++ fff
            s.o = 13;
            s.m = static_cast<int>(f);
        }

        template<Flags f, typename Input, typename State>
        inline void branch_flag_zero(const Input& in, State& s)
        {
            // BZf ++0 fff
            s.o = 12;
            s.m = static_cast<int>(f);
        }

        template<Flags f, typename Input, typename State>
        inline void branch_flag_negative(const Input& in, State& s)
        {
            // BNf ++- fff
            s.o = 11;
            s.m = static_cast<int>(f);
        }
    }

    // Actions for assembler instructions

    template<>
    struct action<in_add>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 4;
            s.m = 9;
            s.t = 0;
            s.op = "add";
        }
    };

    template<>
    struct action<in_adc>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 4;
            s.m = 12;
            s.t = 0;
            s.op = "adc";
        }
    };

    template<>
    struct action<in_sub>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 4;
            s.m = -9;
            s.t = 0;
            s.op = "sub";
        }
    };

    template<>
    struct action<in_sbc>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 4;
            s.m = -8;
            s.t = 0;
            s.op = "sbc";
        }
    };

    template<>
    struct action<in_mul>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 2;
            s.m = 12;
            s.t = 0;
            s.op = "mul";
        }
    };

    template<>
    struct action<in_div>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 2;
            s.m = 9;
            s.t = 0;
            s.op = "div";
        }
    };

    template<>
    struct action<in_min>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 1;
            s.m = 8;
            s.t = 0;
            s.op = "min";
        }
    };

    template<>
    struct action<in_max>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 1;
            s.m = 10;
            s.t = 0;
            s.op = "max";
        }
    };

    template<>
    struct action<in_teq>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 1;
            s.m = 9;
            s.t = 0;
            s.op = "teq";
        }
    };

    template<>
    struct action<in_tem>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 1;
            s.m = 11;
            s.t = 0;
            s.op = "tem";
        }
    };

    template<>
    struct action<in_and> : action<in_min> {};

    template<>
    struct action<in_ort> : action<in_max> {};

    template<>
    struct action<in_adi>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 4;
            s.m = 13;
            s.op = "adi";
        }
    };

    template<>
    struct action<in_sbi>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 4;
            s.m = -5;
            s.op = "sbi";
        }
    };

    template<>
    struct action<in_mli>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 2;
            s.m = -6;
            s.op = "mli";
        }
    };

    template<>
    struct action<in_dvi>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 2;
            s.m = -9;
            s.op = "dvi";
        }
    };

    template<>
    struct action<in_pad>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 4;
            s.m = 11;
            s.op = "pad";
        }
    };

    template<>
    struct action<in_psb>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 4;
            s.m = -7;
            s.op = "psb";
        }
    };

    template<>
    struct action<in_pmu>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 2;
            s.m = -8;
            s.op = "pmu";
        }
    };

    template<>
    struct action<in_pdv>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 2;
            s.m = -10;
            s.op = "pdv";
        }
    };

    template<>
    struct action<in_ret>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // RET: +0+ ---
            s.o = 10;
            s.m = -13;
            s.t = s.x = s.y = s.z = 0;
            s.op = "ret";
        }
    };

    template<>
    struct action<in_srt>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // SRT: 000 ---
            s.m = -13;
            s.o = s.t = s.x = s.y = s.z = 0;
            s.op = "srt";
        }
    };

    template<>
    struct action<in_brk>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // BRK: 000 --0
            s.m = -12;
            s.o = s.t = s.x = s.y = s.z = 0;
            s.op = "brk";
        }
    };

    template<>
    struct action<in_nop>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // NOP = 000 --+
            s.m = -11;
            s.o = s.t = s.x = s.y = s.z = 0;
            s.op = "nop";
        }
    };

    template<>
    struct action<in_und>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // UND = 000 000
            s.o = s.m = s.t = s.x = s.y = s.z = 0;
            s.op = "und";
        }
    };

    template<>
    struct action<in_pfc>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "pfc";
            return detail::set_flag_positive<Flags::carry>(in, s);
        }
    };

    template<>
    struct action<in_pfs>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "pfs";
            return detail::set_flag_positive<Flags::sign>(in, s);
        }
    };

    template<>
    struct action<in_pfd>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "pfd";
            return detail::set_flag_positive<Flags::direction>(in, s);
        }
    };

    template<>
    struct action<in_pfa>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "pfa";
            return detail::set_flag_positive<Flags::absolute>(in, s);
        }
    };

    template<>
    struct action<in_pfb>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "pfb";
            return detail::set_flag_positive<Flags::binary>(in, s);
        }
    };

    template<>
    struct action<in_pft>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "pft";
            return detail::set_flag_positive<Flags::trap>(in, s);
        }
    };

    template<>
    struct action<in_pfi>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "pfi";
            return detail::set_flag_positive<Flags::interrupt>(in, s);
        }
    };

    template<>
    struct action<in_pfp>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "pfp";
            return detail::set_flag_positive<Flags::protect>(in, s);
        }
    };

    template<>
    struct action<in_zfc>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "zfc";
            return detail::set_flag_zero<Flags::carry>(in, s);
        }
    };

    template<>
    struct action<in_zfs>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "zfs";
            return detail::set_flag_zero<Flags::sign>(in, s);
        }
    };

    template<>
    struct action<in_zfd>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "zfd";
            return detail::set_flag_zero<Flags::direction>(in, s);
        }
    };

    template<>
    struct action<in_zfa>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "zfa";
            return detail::set_flag_zero<Flags::absolute>(in, s);
        }
    };

    template<>
    struct action<in_zfb>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "zfb";
            return detail::set_flag_zero<Flags::binary>(in, s);
        }
    };

    template<>
    struct action<in_zft>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "zft";
            return detail::set_flag_zero<Flags::trap>(in, s);
        }
    };

    template<>
    struct action<in_zfi>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "zfi";
            return detail::set_flag_zero<Flags::interrupt>(in, s);
        }
    };

    template<>
    struct action<in_zfp>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "zfp";
            return detail::set_flag_zero<Flags::protect>(in, s);
        }
    };

    template<>
    struct action<in_nfc>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "nfc";
            return detail::set_flag_negative<Flags::carry>(in, s);
        }
    };

    template<>
    struct action<in_nfs>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "nfs";
            return detail::set_flag_negative<Flags::sign>(in, s);
        }
    };

    template<>
    struct action<in_nfd>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "nfd";
            return detail::set_flag_negative<Flags::direction>(in, s);
        }
    };

    template<>
    struct action<in_nfa>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "nfa";
            return detail::set_flag_negative<Flags::absolute>(in, s);
        }
    };

    template<>
    struct action<in_nfb>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "nfb";
            return detail::set_flag_negative<Flags::binary>(in, s);
        }
    };

    template<>
    struct action<in_nft>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "nft";
            return detail::set_flag_negative<Flags::trap>(in, s);
        }
    };

    template<>
    struct action<in_nfi>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "nfi";
            return detail::set_flag_negative<Flags::interrupt>(in, s);
        }
    };

    template<>
    struct action<in_nfp>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "nfp";
            return detail::set_flag_negative<Flags::protect>(in, s);
        }
    };

    template<>
    struct action<in_spc>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "spc";
            return detail::skip_flag_positive<Flags::carry>(in, s);
        }
    };

    template<>
    struct action<in_sps>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "sps";
            return detail::skip_flag_positive<Flags::sign>(in, s);
        }
    };

    template<>
    struct action<in_spd>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "spd";
            return detail::skip_flag_positive<Flags::direction>(in, s);
        }
    };

    template<>
    struct action<in_spa>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "spa";
            return detail::skip_flag_positive<Flags::absolute>(in, s);
        }
    };

    template<>
    struct action<in_spb>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "spb";
            return detail::skip_flag_positive<Flags::binary>(in, s);
        }
    };

    template<>
    struct action<in_spt>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "spt";
            return detail::skip_flag_positive<Flags::trap>(in, s);
        }
    };

    template<>
    struct action<in_spi>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "spi";
            return detail::skip_flag_positive<Flags::interrupt>(in, s);
        }
    };

    template<>
    struct action<in_spp>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "spp";
            return detail::skip_flag_positive<Flags::protect>(in, s);
        }
    };

    template<>
    struct action<in_szc>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "szc";
            return detail::skip_flag_zero<Flags::carry>(in, s);
        }
    };

    template<>
    struct action<in_szs>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "szs";
            return detail::skip_flag_zero<Flags::sign>(in, s);
        }
    };

    template<>
    struct action<in_szd>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "szd";
            return detail::skip_flag_zero<Flags::direction>(in, s);
        }
    };

    template<>
    struct action<in_sza>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "sza";
            return detail::skip_flag_zero<Flags::absolute>(in, s);
        }
    };

    template<>
    struct action<in_szb>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "szb";
            return detail::skip_flag_zero<Flags::binary>(in, s);
        }
    };

    template<>
    struct action<in_szt>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "szt";
            return detail::skip_flag_zero<Flags::trap>(in, s);
        }
    };

    template<>
    struct action<in_szi>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "szi";
            return detail::skip_flag_zero<Flags::interrupt>(in, s);
        }
    };

    template<>
    struct action<in_szp>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "szp";
            return detail::skip_flag_zero<Flags::protect>(in, s);
        }
    };

    template<>
    struct action<in_snc>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "snc";
            return detail::skip_flag_negative<Flags::carry>(in, s);
        }
    };

    template<>
    struct action<in_sns>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "sns";
            return detail::skip_flag_negative<Flags::sign>(in, s);
        }
    };

    template<>
    struct action<in_snd>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "snd";
            return detail::skip_flag_negative<Flags::direction>(in, s);
        }
    };

    template<>
    struct action<in_sna>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "sna";
            return detail::skip_flag_negative<Flags::absolute>(in, s);
        }
    };

    template<>
    struct action<in_snb>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "snb";
            return detail::skip_flag_negative<Flags::binary>(in, s);
        }
    };

    template<>
    struct action<in_snt>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "snt";
            return detail::skip_flag_negative<Flags::trap>(in, s);
        }
    };

    template<>
    struct action<in_sni>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "sni";
            return detail::skip_flag_negative<Flags::interrupt>(in, s);
        }
    };

    template<>
    struct action<in_snp>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "snp";
            return detail::skip_flag_negative<Flags::protect>(in, s);
        }
    };

    template<>
    struct action<in_inc>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // INC rX 0++ ++- RRR 000 000 00+
            s.o = 4;
            s.m = 11;
            s.z = 1;
            s.x = s.y = 0;
            s.op = "inc";
        }
    };

    template<>
    struct action<in_dec>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // DEC rX 0++ -0+ RRR 000 000 00+
            s.o = 4;
            s.m = -8;
            s.x = s.y = 0;
            s.op = "dec";
        }
    };

    template<>
    struct action<in_clr>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // CLR rX +0- 00+ 000 000 XXX 000
            s.o = 8;
            s.m = 1;
            s.t = 0;
            s.x = s.z = 0;
            s.op = "clr";
        }
    };

    template<>
    struct action<in_srz> : action<in_clr> {};

    template<>
    struct action<in_srp>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // SRP rX +0- 00+ 00+ 000 XXX 000
            s.o = 8;
            s.m = 1;
            s.t = 1;
            s.x = s.z = 0;
            s.op = "srp";
        }
    };
    
    template<>
    struct action<in_srn>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // SRN rX +0- 00+ 00- 000 XXX 000
            s.o = 8;
            s.m = 1;
            s.t = -1;
            s.x = s.z = 0;
            s.op = "srp";
        }
    };

    template<>
    struct action<in_psh>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // PSH rX +0- +++ 000 000 000 XXX
            s.o = 8;
            s.m = 13;
            s.t = s.x = s.y = 0;
            s.op = "psh";
        }
    };

    template<>
    struct action<in_pop>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // POP rX +0- ++- 000 000 000 XXX
            s.o = 8;
            s.m = 11;
            s.t = s.x = s.y = 0;
            s.op = "pop";
        }
    };

    template<>
    struct action<in_sti>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // STI rX 00+ 000 000 000 000 RRR
            s.o = 1;
            s.m = 0;
            s.t = s.x = s.y = 0;
            s.op = "sti";
        }
    };

    template<>
    struct action<in_not> : action<in_sti> {};

    template<>
    struct action<in_neg> : action<in_sti> {};

    template<>
    struct action<in_pti>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // PTI rX 00+ 00+ 000 000 000 RRR
            s.o = 1;
            s.m = 1;
            s.t = s.x = s.y = 0;
            s.op = "pti";
        }
    };

    template<>
    struct action<in_nti>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // NTI rX 00+ 00- 000 000 000 RRR
            s.o = 1;
            s.m = -1;
            s.t = s.x = s.y = 0;
            s.op = "nti";
        }
    };

    template<>
    struct action<in_mov>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // MOV rX, rY +0- 00- ... ... XXX YYY
            s.o = 8;
            s.m = -1;
            s.t = s.x = 0;
            s.op = "mov";
        }
    };

    template<>
    struct action<in_xch>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // XCH rX, rY +0- 0-- ... ... XXX YYY
            s.o = 8;
            s.m = -4;
            s.t = s.x = 0;
            s.op = "xch";
        }
    };

    template<>
    struct action<in_cmp>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // CMP rX, rY 00+ -00 ... ... XXX YYY
            s.o = 1;
            s.m  = -9;
            s.t = s.x = 0;
            s.op = "cmp";
        }
    };

    template<>
    struct action<in_bin>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // BIN rX, rY 0+- 00+ ... ... XXX YYY
            s.o = 2;
            s.m = 1;
            s.t = s.x = 0;
            s.op = "bin";
        }
    };

    template<>
    struct action<in_tri>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // TRI rX, rY 0+- 00- ... ... XXX YYY
            s.o = 2;
            s.m = -1;
            s.t = s.x = 0;
            s.op = "tri";
        }
    };

    template<>
    struct action<in_fdr>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // FDR rX, rY 0+- 0++ ... ... XXX YYY
            s.o = 2;
            s.m = 4;
            s.t = s.x = 0;
            s.op = "fdr";
        }
    };

    template<>
    struct action<in_rdr>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // RDR rX, rY 0+- 0+- ... ... XXX YYY
            s.o = 2;
            s.m = 2;
            s.t = s.x = 0;
            s.op = "rdr";
        }
    };

    template<>
    struct action<in_brr>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // BRR rX +0+ 0+0 ... ... ... RRR
            s.o = 10;
            s.m = 3;
            s.t = s.x = s.y = 0;
            s.op = "brr";
        }
    };

    template<>
    struct action<in_caa>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // CAA rX +0+ 0-0 ... ... ... RRR
            s.o = 10;
            s.m = 3;
            s.t = s.x = s.y = 0;
            s.op = "caa";
        }
    };

    template<>
    struct action<in_sys>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // SYS #vec 000 +++ ... ... VVV VVV
            s.o = 0;
            s.m = 13;
            s.t = s.x;
            s.op = "sys";
        }
    };

    template<>
    struct action<in_int>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // INT @io, rX -0+ 00+ RRR PPP PPP PPP
            s.o = -8;
            s.m = 1;
            s.op = "int";
        }
    };

    template<>
    struct action<in_inb>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // INB @io, rX -0+ 0++ RRR PPP PPP PPP
            s.o = -8;
            s.m = 4;
            s.op = "inb";
        }
    };

    template<>
    struct action<in_out>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // OUT rX, @io -0+ 00- RRR PPP PPP PPP
            s.o = -8;
            s.m = -1;
            s.op = "out";
        }
    };

    template<>
    struct action<in_oub>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // OUB rX, @io -0+ 0+- RRR PPP PPP PPP
            s.o = 8;
            s.m = 2;
            s.op = "oub";
        }
    };

    template<>
    struct action<in_cmi>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // CMI, rX, imm 00+ -0+ ... XXX iii iii
            s.o = 1;
            s.m = -8;
            s.t = 0;
            s.op = "cmi";
        }
    };

    template<>
    struct action<in_shl>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // SHL, rX, imm 00+ --+ ... XXX iii iii
            s.o = 1;
            s.m = -11;
            s.t = 0;
            s.op = "shl";
        }
    };

    template<>
    struct action<in_shr>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // SHR, rX, imm 00+ --- ... XXX iii iii
            s.o = 1;
            s.m = -13;
            s.t = 0;
            s.op = "shr";
        }
    };

    template<>
    struct action<in_rol>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // ROL, rX, imm 00+ -++ ... XXX iii iii
            s.o = 1;
            s.m = -5;
            s.t = 0;
            s.op = "rol";
        }
    };

    template<>
    struct action<in_ror>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // ROR, rX, imm 00+ -+- ... XXX iii iii
            s.o = 1;
            s.m = -7;
            s.t = 0;
            s.op = "ror";
        }
    };

    template<>
    struct action<in_rcl>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // RCL, rX, imm 00+ --0 ... XXX iii iii
            s.o = 1;
            s.m = -12;
            s.t = 0;
            s.op = "rcl";
        }
    };

    template<>
    struct action<in_rcr>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // CMI, rX, imm 00+ -0- ... XXX iii iii
            s.o = 1;
            s.m = -10;
            s.t = 0;
            s.op = "rcr";
        }
    };

    template<>
    struct action<in_bri>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // BRI, rX, imm +0+ 0+- ... XXX iii iii
            s.o = 10;
            s.m = 2;
            s.t = 0;
            s.op = "bri";
        }
    };

    template<>
    struct action<in_bra>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // BRA mem +0+ +00
            s.o = 10;
            s.m = 9;
            s.op = "bra";
        }
    };

    template<>
    struct action<in_brs>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // BRS mem +0+ 00- ... ...
            s.o = 10;
            s.m = -1;
            s.t = s.x = 0;
            s.op = "brs";
        }
    };

    template<>
    struct action<in_brl>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // BRL mem +0+ 00+
            s.o = 10;
            s.m = 1;
            s.op = "brl";
        }
    };

    template<>
    struct action<in_cal>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // CAL mem +0+ 000
            s.o = 10;
            s.m = 0;
            s.op = "cal";
        }
    };

    template<>
    struct action<in_bpc>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bpc";
            return detail::branch_flag_positive<Flags::carry>(in, s);
        }
    };

    template<>
    struct action<in_bps>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bps";
            return detail::branch_flag_positive<Flags::sign>(in, s);
        }
    };

    template<>
    struct action<in_bpd>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bpd";
            return detail::branch_flag_positive<Flags::direction>(in, s);
        }
    };

    template<>
    struct action<in_bpa>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bpa";
            return detail::branch_flag_positive<Flags::absolute>(in, s);
        }
    };

    template<>
    struct action<in_bpb>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bpb";
            return detail::branch_flag_positive<Flags::binary>(in, s);
        }
    };

    template<>
    struct action<in_bpt>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bpt";
            return detail::branch_flag_positive<Flags::trap>(in, s);
        }
    };

    template<>
    struct action<in_bpi>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bpi";
            return detail::branch_flag_positive<Flags::interrupt>(in, s);
        }
    };

    template<>
    struct action<in_bpp>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bpp";
            return detail::branch_flag_positive<Flags::protect>(in, s);
        }
    };

    template<>
    struct action<in_bzc>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bzc";
            return detail::branch_flag_zero<Flags::carry>(in, s);
        }
    };

    template<>
    struct action<in_bzs>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bzs";
            return detail::branch_flag_zero<Flags::sign>(in, s);
        }
    };

    template<>
    struct action<in_bzd>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bzd";
            return detail::branch_flag_zero<Flags::direction>(in, s);
        }
    };

    template<>
    struct action<in_bza>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bza";
            return detail::branch_flag_zero<Flags::absolute>(in, s);
        }
    };

    template<>
    struct action<in_bzb>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bzb";
            return detail::branch_flag_zero<Flags::binary>(in, s);
        }
    };

    template<>
    struct action<in_bzt>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bzt";
            return detail::branch_flag_zero<Flags::trap>(in, s);
        }
    };

    template<>
    struct action<in_bzi>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bzi";
            return detail::branch_flag_zero<Flags::interrupt>(in, s);
        }
    };

    template<>
    struct action<in_bzp>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bzp";
            return detail::branch_flag_zero<Flags::protect>(in, s);
        }
    };

    template<>
    struct action<in_bnc>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bnc";
            return detail::branch_flag_negative<Flags::carry>(in, s);
        }
    };

    template<>
    struct action<in_bns>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bns";
            return detail::branch_flag_negative<Flags::sign>(in, s);
        }
    };

    template<>
    struct action<in_bnd>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bnd";
            return detail::branch_flag_negative<Flags::direction>(in, s);
        }
    };

    template<>
    struct action<in_bna>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bna";
            return detail::branch_flag_negative<Flags::absolute>(in, s);
        }
    };

    template<>
    struct action<in_bnb>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bnb";
            return detail::branch_flag_negative<Flags::binary>(in, s);
        }
    };

    template<>
    struct action<in_bnt>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bnt";
            return detail::branch_flag_negative<Flags::trap>(in, s);
        }
    };

    template<>
    struct action<in_bni>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bni";
            return detail::branch_flag_negative<Flags::interrupt>(in, s);
        }
    };

    template<>
    struct action<in_bnp>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.op = "bnp";
            return detail::branch_flag_negative<Flags::protect>(in, s);
        }
    };

    template<>
    struct action<in_ldr>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 9;
            s.op = "ldr";
        }
    };

    template<>
    struct action<in_ldl>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 5;
            s.op = "ldl";
        }
    };

    template<>
    struct action<in_ldm>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 6;
            s.op = "ldm";
        }
    };

    template<>
    struct action<in_ldh>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 7;
            s.op = "ldh";
        }
    };

    template<>
    struct action<in_ldi>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 8;
            s.m = 0;
            s.t = 0;
            s.op = "ldi";
        }
    };

    template<>
    struct action<in_lil>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 8;
            s.m = 2;
            s.t = 0;
            s.op = "lil";
        }
    };

    template<>
    struct action<in_lim>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 8;
            s.m = 3;
            s.t = 0;
            s.op = "lim";
        }
    };

    template<>
    struct action<in_lih>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 8;
            s.m = 4;
            s.t = 0;
            s.op = "lih";
        }
    };

    template<>
    struct action<in_ldn>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 8;
            s.m = 10;
            s.t = 0;
            s.op = "ldn";
        }
    };

    template<>
    struct action<in_str>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = -9;
            s.op = "str";
        }
    };

    template<>
    struct action<in_stl>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = -13;
            s.op = "stl";
        }
    };

    template<>
    struct action<in_stm>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = -12;
            s.op = "stm";
        }
    };

    template<>
    struct action<in_sth>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = -11;
            s.op = "sth";
        }
    };

    template<>
    struct action<in_stn>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 8;
            s.m = -8;
            s.t = 0;
            s.op = "stn";
        }
    };

    template<>
    struct action<in_lda>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 8;
            s.m = 9;
            s.t = s.x = 0;
            s.op = "lda";
        }
    };

    template<>
    struct action<in_lal>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 8;
            s.m = 5;
            s.t = s.x = 0;
            s.op = "lal";
        }
    };

    template<>
    struct action<in_lam>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 8;
            s.m = 6;
            s.t = s.x = 0;
            s.op = "lam";
        }
    };

    template<>
    struct action<in_lah>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 8;
            s.m = 7;
            s.t = s.x = 0;
            s.op = "lah";
        }
    };

    template<>
    struct action<in_sta>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = -10;
            s.m = 9;
            s.t = s.x = 0;
            s.op = "sta";
        }
    };

    template<>
    struct action<in_sal>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = -10;
            s.m = 5;
            s.t = s.x = 0;
            s.op = "sal";
        }
    };

    template<>
    struct action<in_sam>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = -10;
            s.m = 6;
            s.t = s.x = 0;
            s.op = "sam";
        }
    };

    template<>
    struct action<in_sah>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = -10;
            s.m = 7;
            s.t = s.x = 0;
            s.op = "sah";
        }
    };

    template<>
    struct action<in_lsr>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 0;
            s.m = 1;
            s.t = s.y = 0;
            s.op = "lsr";
        }
    };

    template<>
    struct action<in_ssr>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            s.o = 0;
            s.m = -1;
            s.t = s.y = 0;
            s.op = "ssr";
        }
    };

    template<>
    struct action<arithmetic_basic>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Arithmetic basic instructions can have
            // 1-3 register operands, filling in the X,Y,Z fields
            s.x = s.operands.front();
            s.operands.pop();

            if (s.operands.empty())
            {
                s.y = s.z = s.x;
            }
            else
            {
                s.y = s.operands.front();
                s.operands.pop();

                if (s.operands.empty())
                {
                    s.z = s.y;
                }
                else
                {
                    s.z = s.operands.front();
                    s.operands.pop();
                }
            }
        }
    };

    template<>
    struct action<arithmetic_immediate>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Registers in T,X; immediate in Y:Z
            // But we might only have 1 register operand
            s.t = s.operands.front();
            s.operands.pop();

            if (s.operands.size() > 1)
            {
                s.x = s.operands.front();
                s.operands.pop();
            }
            else
            {
                s.x = s.t;
            }

            auto imm { s.operands.front() };
            s.operands.pop();

            s.z = low_trits(imm, 3);
            s.y = shift_right(imm, 3);
        }
    };

    template<>
    struct action<arithmetic_inplace>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Register in T field, immediate in X,Y,Z
            s.t = s.operands.front();
            s.operands.pop();

            auto imm { s.operands.front() };
            s.operands.pop();

            s.x = shift_right(imm, 6);
            s.y = low_trits(shift_right(imm, 3), 3);
            s.z = low_trits(imm, 3);
        }
    };

    template<>
    struct action<logical_single_t>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Put single operand in T field of opcode
            s.t = s.operands.front();
            s.operands.pop();
        }
    };

    template<>
    struct action<logical_single_y>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Put single operand in Y field of opcode
            s.y = s.operands.front();
            s.operands.pop();
        }
    };

    template<>
    struct action<logical_single_z>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Put single operand in Z field of opcode
            s.z = s.operands.front();
            s.operands.pop();
        }
    };

    template<>
    struct action<logical_double>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Put 2 operands in Y and Z fields of opcode
            s.y = s.operands.front();
            s.operands.pop();
            s.z = s.operands.front();
            s.operands.pop();
        }
    };

    template<>
    struct action<logical_immediate>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Register operand in X, then 6-trit immediate in Y:Z
            s.x = s.operands.front();
            s.operands.pop();

            auto imm { s.operands.front() };
            s.operands.pop();

            s.y = shift_right(imm, 3);
            s.z = low_trits(imm, 3);
        }
    };

    template<>
    struct action<load_basic>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Memory operand in T,X,Y,Z fields, register in M
            auto memaddr { s.operands.front() };
            s.operands.pop();

            s.z = low_trits(memaddr, 3);
            memaddr = shift_right(memaddr, 3);
            s.y = low_trits(memaddr, 3);
            memaddr = shift_right(memaddr, 3);
            s.x = low_trits(memaddr, 3);
            memaddr = shift_right(memaddr, 3);
            s.t = memaddr;

            s.m = s.operands.front();
            s.operands.pop();
        }
    };

    template<>
    struct action<load_immediate>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Register in X, immediate in Y:Z
            s.x = s.operands.front();
            s.operands.pop();

            auto imm { s.operands.front() };
            s.operands.pop();

            s.z = low_trits(imm, 3);
            s.y = shift_right(imm, 3);
        }
    };

    template<>
    struct action<load_indexed>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Memory operand in Y:Z, register in X
            auto memaddr { s.operands.front() };
            s.operands.pop();

            s.z = low_trits(memaddr, 3);
            s.y = shift_right(memaddr, 3);

            s.x = s.operands.front();
            s.operands.pop();
        }
    };

    template<>
    struct action<store_basic>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Register operand in M, memory address in T,X,Y,Z
            s.m = s.operands.front();
            s.operands.pop();

            auto memaddr { s.operands.front() };
            s.operands.pop();

            s.z = low_trits(memaddr, 3);
            memaddr = shift_right(memaddr, 3);
            s.y = low_trits(memaddr, 3);
            memaddr = shift_right(memaddr, 3);
            s.x = low_trits(memaddr, 3);
            memaddr = shift_right(memaddr, 3);
            s.t = memaddr;
        }
    };

    template<>
    struct action<store_indexed>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Register in X, memory operand in Y:Z
            s.x = s.operands.front();
            s.operands.pop();            

            auto memaddr { s.operands.front() };
            s.operands.pop();

            s.z = low_trits(memaddr, 3);
            s.y = shift_right(memaddr, 3);
        }
    };

    template<>
    struct action<load_store_indirect>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Registers in Y and Z
            s.y = s.operands.front();
            s.operands.pop();

            s.z = s.operands.front();
            s.operands.pop();
        }
    };

    template<>
    struct action<branch_vector>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // One operand fills the Y and Z fields
            auto vec { s.operands.front() };
            s.operands.pop();

            s.z = low_trits(vec, 3);
            s.y = shift_right(vec, 3);
        }
    };

    template<>
    struct action<input>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Register operand in T field, then I/O address in X,Y,Z
            auto ioaddr { s.operands.front() };
            s.operands.pop();

            s.x = shift_right(ioaddr, 6);
            s.y = low_trits(shift_right(ioaddr, 3), 3);
            s.z = low_trits(ioaddr, 3);

            s.t = s.operands.front();
            s.operands.pop();
        }
    };

    template<>
    struct action<output>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Register operand in T field, then I/O address in X,Y,Z
            // Output has a different operand order, so account for that.
            s.t = s.operands.front();
            s.operands.pop();

            auto ioaddr { s.operands.front() };
            s.operands.pop();

            s.x = shift_right(ioaddr, 6);
            s.y = low_trits(shift_right(ioaddr, 3), 3);
            s.z = low_trits(ioaddr, 3);
        }
    };

    template<>
    struct action<branch_basic>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // 12-trit memory address
            // (BRS instruction only uses 6, but that's okay.)
            auto memaddr { s.operands.front() };
            s.operands.pop();

            s.z = low_trits(memaddr, 3);
            memaddr = shift_right(memaddr, 3);
            s.y = low_trits(memaddr, 3);
            memaddr = shift_right(memaddr, 3);
            s.x = low_trits(memaddr, 3);
            memaddr = shift_right(memaddr, 3);
            s.t = memaddr;
        }
    };
    
    // For now, this does the same thing as the above.
    // The actions for the individual parts of the isntruction
    // handle all the internal logic.
    template<>
    struct action<branch_offset> : action<branch_basic> {};

    template<>
    struct action<load_system>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Register in X, system register in Z
            s.x = s.operands.front();
            s.operands.pop();

            s.z = s.operands.front();
            s.operands.pop();
        }
    };

    template<>
    struct action<store_system>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            // Operands are reversed from load version,
            // but still placed in the same fields
            s.z = s.operands.front();
            s.operands.pop();

            s.x = s.operands.front();
            s.operands.pop();
        }
    };

    // These do the same thing for now, but we may need to change later
    template<>
    struct action<move_basic> : action<logical_double> {};

    template<>
    struct action<branch_indirect> : action<logical_single_z> {};

    template<>
    struct action<branch_immediate> : action<logical_immediate> {};
}}

#endif /* TRIREME_INSTRUCTION_ACTIONS_HPP */