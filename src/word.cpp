#include <string>

#include "word.hpp"
#include "ternary_math.hpp"

namespace ternary
{
    std::string Word::value_string() const noexcept
    {
        std::string result { "%" };
        result += high.value_string();
        result += middle.value_string();
        result += low.value_string();
        
        return result;
    }

    std::string Word::trit_string() const noexcept
    {
        auto result { high.trit_string() };
        result += " ";
        result += middle.trit_string();
        result += " ";
        result += low.trit_string();

        return result;
    }
}
