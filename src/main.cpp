#include <iostream>
#include <array>
#include <string>

#include "convert.hpp"
#include "ternary_math.hpp"
#include "hexad.hpp"
#include "word.hpp"
#include "detail/convert_impl.hpp"
#include "detail/character_to_value.hpp"

#include "assembler/assembler.hpp"

using namespace ternary;

template<typename T, std::size_t N>
void print_array(const std::array<T,N>& arr)
{
    for (auto&& v : arr)
    {
        std::cout << v << ',';
    }
}

std::ostream& operator<<(std::ostream& os, Hexad h)
{
    return os << "H" << h.get();
}

std::ostream& operator<<(std::ostream& os, Word w)
{
    return os << w.value_string();
}

template<typename T, typename U>
std::ostream& operator<<(std::ostream& os, std::pair<T, U> pair)
{
    return os << pair.first << ',' << pair.second;
}

struct test_state
{
    int converted;
    int o,m,t,x,y,z;
    std::string op;
};


int main(int, char**) {
    using namespace std::string_literals;

    std::string s { "1234 ; comment\n-4321\n321 ; comment -999\n555;    +999" };
    tao::pegtl::string_input<> in { s, "testing" };

    std::string sample_filename { "sandbox/sample.txt" };
    tao::pegtl::file_input<> fin { sample_filename };

    auto analysis { ternary::assembler::analyze() };

    if (!analysis)
    {
        test_state ts {};
        std::cout << ternary::assembler::assemble(fin, ts) << '\n';
        std::cout << ts.converted << '\n';
    }
}
