#include "flags.hpp"

namespace ternary {
    std::string FlagRegister::to_string() const
    {
        // Each string is identified by a single character,
        // with reserved flags using spaces.
        const std::string all_flags { "        PITBA  DSC" };
        const std::string trits { w_.raw_trit_string() };

        return std::inner_product(
            trits.crbegin(), trits.crend(),
            all_flags.cbegin(),
            std::string {},
            std::plus<>(),
            [](char t, char f) { 
                if (f != ' ')
                {
                    return std::string({ t, f, ' ' });
                }
                else
                {
                    return std::string {};
                }
            }
        );
    }
}