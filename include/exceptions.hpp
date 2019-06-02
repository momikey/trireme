#ifndef TRIREME_EXCEPTIONS_HPP
#define TRIREME_EXCEPTIONS_HPP

#include <exception>

/*
 * We can use exceptions to represent system interrupts, as these are called
 * only in exceptional circumstances, never directly by user code. (The `und`
 * instruction is an *intentional* exception to this rule.)
 */
namespace ternary
{
    // First, a base class; we won't instantiate this, but we *will* catch it.
    class system_interrupt_base
    {
        public:
        // Each interrupt will have its own value, which determines the vector
        // the CPU jumps to when that interrupt occurs.
        virtual int value() const = 0;
    };

    // Template tricks - we template on the interrupt number, which makes
    // using it for a vector table that much easier.
    template<int I>
    class system_interrupt : public system_interrupt_base
    {
        public:
        virtual int value() const override { return I; }
    };

    // Convenience declarations for the defined interrutps
    using divide_by_zero = system_interrupt<0>;
    using debug_breakpoint = system_interrupt<1>;
    using protection_violation = system_interrupt<2>;
    using invalid_flag = system_interrupt<3>;
    using invalid_opcode = system_interrupt<4>;
}

#endif /* TRIREME_EXCEPTIONS_HPP */