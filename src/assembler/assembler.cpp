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

    Assembler::data_map Assembler::assemble_line(std::string in, int origin)
    {
        try
        {
            // Copy the existing state, but clear the output map.
            // This lets us keep things like labels for a whole session.
            state immediate_state { st_ };
            immediate_state.data.clear();

            auto result { assemble_line(in, immediate_state, origin) };
            
            if (result)
            {
                // An immediate instruction in the shell may change the
                // symbol table, so we have to account for that.
                // (In C++17, we can do this with a simple call to `merge`.
                // No such luck in C++14.)
                for (auto&& kv : immediate_state.symbol_table)
                {
                    st_.symbol_table[kv.first] = kv.second;
                }

                return immediate_state.data;
                // Assembler::data_map line;
                // std::copy_if(st_.data.begin(), st_.data.end(),
                //     std::inserter(line, line.end()),
                //     [origin](auto v){ return v.first >= origin; });

                // return line;
            }
            
        }
        catch(const tao::pegtl::parse_error& e)
        {
            const auto p { e.positions.front() };
            std::cerr
                << e.what() << '\n'
                << in << '\n'
                << std::string(p.byte_in_line, ' ') << '^' << '\n';          
        }

        return {};
    }
}}
