#include "assembler/assembler.hpp"

namespace ternary { namespace assembler {
    Assembler::data_map Assembler::assemble_file(std::string filename)
    {
        // Parsing consumes the file, and PEGTL doesn't
        // offer a seek method on file inputs, so we have
        // to create it twice.
        tao::pegtl::file_input<> first { filename };

        assert(analyze() == 0);

        try
        {
            first_pass(first, st_);

            tao::pegtl::file_input<> second { filename };
        
            assemble(second, st_);
            return st_.data;
        }
        catch(const tao::pegtl::parse_error& e)
        {
            const auto p = e.positions.front();
            std::cerr
                << e.what() << '\n'
                << first.line_as_string(p) << '\n'
                << std::string(p.byte_in_line, ' ') << '^' << '\n';
        }

        return {};
    }
}}
