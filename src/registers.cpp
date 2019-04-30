#include "registers.hpp"
#include "ternary_math.hpp"

namespace ternary
{
    /**
     * @brief Get the contents of a machine register.
     * 
     * @param r The register number, which will be clamped internally
     * @return Word The value of the register
     */
    Word Registers::get(int r) const noexcept
    {
        if (r == zero_register)
        {
            return 0;
        }

        if (r >= 0)
        {
            return general[clamp_address(r)];
        }
        else
        {
            return special[clamp_address(r)];
        }
    }

    /**
     * @brief Set the contents of a machine register.
     * 
     * @param r The register number, which will be clamped internally
     * @param w The new value of the register
     */
    void Registers::set(int r, const Word& w) noexcept
    {
        if (r == zero_register)
        {
            return;
        }
        
        if (r >= 0)
        {
            general[clamp_address(r)] = w.value();
        }
        else
        {
            special[clamp_address(r)] = w.value();
        }
    }
}