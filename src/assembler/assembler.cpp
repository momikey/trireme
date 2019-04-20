#include "assembler/assembler.hpp"

namespace ternary { namespace assembler {
    bool Assembler::assemble_file(std::string filename)
    {
        // Parsing consumes the file, and PEGTL doesn't
        // offer a seek method on file inputs, so we have
        // to create it twice.
        tao::pegtl::file_input<> first { filename };

        assert(analyze() == 0);

        auto first_result { first_pass(first, st_) };

        if (first_result)
        {
            tao::pegtl::file_input<> second { filename };
            auto second_result { assemble(second, st_) };
            return second_result;
        }
        else
        {
            return false;
        }
        
        return false;
    }
}}
