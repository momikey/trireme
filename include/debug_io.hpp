#ifndef TRIREME_DEBUG_IO_HPP
#define TRIREME_DEBUG_IO_HPP

#include <string>
#include <iostream>

#include "ternary_math.hpp"

namespace ternary
{
    class DebugIo
    {
        public:
        DebugIo() = default;

        char read();
        void write(char c);

        int read_control();
        void write_control(int value);

        private:
        void get_input() { std::getline(std::cin, backing_input); }
        void print() { std::cout << backing_output << '\n'; }

        int request;

        std::string backing_input;
        std::string backing_output;
    };
}

#endif /* TRIREME_DEBUG_IO_HPP */