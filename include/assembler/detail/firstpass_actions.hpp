#ifndef TRIREME_FIRSTPASS_ACTIONS_HPP
#define TRIREME_FIRSTPASS_ACTIONS_HPP

#include <tao/pegtl.hpp>

#include <string>

#include "grammar.hpp"

namespace ternary { namespace assembler {
    using namespace tao::pegtl;

    // Do-nothing rule, as in the main pass
    template<typename Rule>
    struct firstpass : nothing<Rule> {};
}}

#endif /* TRIREME_FIRSTPASS_ACTIONS_HPP */