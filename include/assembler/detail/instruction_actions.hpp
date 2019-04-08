#ifndef TRIREME_INSTRUCTION_ACTIONS_HPP
#define TRIREME_INSTRUCTION_ACTIONS_HPP

#include <tao/pegtl.hpp>

#include <iostream>

#include "instructions.hpp"

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
    }

    // Actions for assembler instructions
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

    // TODO: Testing
    template<>
    struct action<instruction>
    {
        template<typename Input, typename State>
        static void apply(const Input& in, State& s)
        {
            std::cout
                << s.op << '\t'
                << s.o << '\t'
                << s.m << '\t'
                << s.t << '\t'
                << s.x << '\t'
                << s.y << '\t'
                << s.z << '\n';
        }
    };
}}

#endif /* TRIREME_INSTRUCTION_ACTIONS_HPP */