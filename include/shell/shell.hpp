#ifndef TRIREME_SHELL_HPP
#define TRIREME_SHELL_HPP

#include <string>
#include <functional>
#include <iostream>

#include <boost/algorithm/string.hpp>

#include <fmt/format.h>
#include <replxx.hxx>

#include "cpu.hpp"
#include "word.hpp"

#include "assembler/assembler.hpp"

namespace ternary { namespace shell {
    class Shell
    {
        public:
        Shell(Cpu& cpu);

        void start();

        private:
        Cpu& cpu_;
        replxx::Replxx repl_;
        ternary::assembler::Assembler asm_;

        // Holder for "virtual" IP used on command line
        Word ip_;

        // Helper functions
        std::string prompt(const std::string& continuation);

        void assemble_instruction(const std::string& input);
        void handle_command(const std::string& input);
    };
}}

#endif /* TRIREME_SHELL_HPP */