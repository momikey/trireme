#ifndef TRIREME_SHELL_HPP
#define TRIREME_SHELL_HPP

#include <string>
#include <functional>
#include <iostream>
#include <map>
#include <regex>

#include <boost/algorithm/string.hpp>

#include <fmt/format.h>
#include <replxx.hxx>

#include "cpu.hpp"
#include "word.hpp"
#include "convert.hpp"

#include "assembler/assembler.hpp"

namespace ternary { namespace shell {
    class Shell
    {
        public:
        using ShellCommand = std::function<bool(const std::string&)>;

        Shell(Cpu& cpu);

        void start();

        private:
        Cpu& cpu_;
        replxx::Replxx repl_;
        std::map<std::string, ShellCommand> commands;
        ternary::assembler::Assembler asm_;

        // Holder for "virtual" IP used on command line
        Word ip_;

        // Helper functions
        std::string prompt(const std::string& continuation);

        void assemble_instruction(const std::string& input);
        void handle_command(const std::string& input);

        bool handle_register(const std::string& rest);
    };

    // Convenience regexes
    static const std::string register_name { R"(r([A-Mn-z0]))" };
    static const std::string balanced_integer { R"((?:[+-]?[0-9]{1,9})|(?:%[A-Mn-z0]{1,6}))" };
    static const std::regex match_register { fmt::format(R"({0}(?:\s*=\s*({1}))?)",
        register_name, balanced_integer) };
    static const std::regex match_memory { fmt::format(R"(({0})(?:\s*=\s*({1}))?)",
        balanced_integer, balanced_integer) };
}}

#endif /* TRIREME_SHELL_HPP */