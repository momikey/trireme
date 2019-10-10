#ifndef TRIREME_DEBUG_IO_HPP
#define TRIREME_DEBUG_IO_HPP

#include <string>
#include <iostream>

#include "ternary_math.hpp"
#include "word.hpp"

namespace ternary
{
    class DebugIo
    {
        public:
        DebugIo() = default;

        char read();
        void write(int input);

        int read_control();
        void write_control(int value);

        private:
        void get_input() { std::getline(std::cin, backing_input); }
        void print() { std::cout << backing_output << '\n'; }
        void print_and_clear()
        {
            print();
            backing_output.clear();
        }

        // Whether the debug I/O should request input
        int request = 0;

        // Whether the debug I/O should output characters or integers
        int mode = 0;

        std::string backing_input;
        std::string backing_output;
    };
}

#endif /* TRIREME_DEBUG_IO_HPP */