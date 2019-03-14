#ifndef TRIREME_HEXAD_HPP
#define TRIREME_HEXAD_HPP

#include <cstddef>

#include "ternary_math.hpp"

namespace ternary {
    class Hexad
    {
        using value_type = int;

    private:
        value_type value;
    public:
        const std::size_t width = 6ul;

        Hexad(value_type v_): value(low_trits(v_, width)) {}


    };
}

#endif /* TRIREME_HECAD_HPP */