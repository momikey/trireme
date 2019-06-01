#ifndef TRIREME_EXCEPTIONS_HPP
#define TRIREME_EXCEPTIONS_HPP

#include <exception>

/*
 * We can use exceptions to represent system interrupts, as these are called
 * only in exceptional circumstances, never directly by user code.
 */
namespace ternary
{
    // First, a base class (we won't instantiate this)
    class system_interrupt_base
    {
        virtual int value() = 0;
    };

    // Template tricks
    template<int I>
    class system_interrupt : system_interrupt_base
    {
        virtual int value() override { return I; }
    };

    using divide_by_zero = system_interrupt<0>;
    using debug_breakpoint = system_interrupt<1>;
    using protection_violation = system_interrupt<2>;
    using invalid_flag = system_interrupt<3>;
}

#endif /* TRIREME_EXCEPTIONS_HPP */