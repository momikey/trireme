#ifndef TRIREME_IO_HPP
#define TRIREME_IO_HPP

#include <array>
#include <map>
#include <functional>

#include "word.hpp"
#include "ternary_math.hpp"

namespace ternary
{
    struct Io
    {
        // I/O handlers 
        using read_handler_t = std::function<int()>;
        using write_handler_t = std::function<void(int)>;

        static constexpr auto io_address_width = 9;

        public:
        Io() = default;

        // TODO: Implement a kind of binding system here.
        Word read(int address);
        Word read(Word address) { return read(address.value()); }
        int read_binary(int address);
        int read_binary(Word address) { return read_binary(address.value()); }

        void write(int address, int data);
        void write(Word address, Word data) { return write(address.value(), data.value()); }
        void write_binary(int address, unsigned char data);
        void write_binary(Word address, unsigned char data) { return write(address.value(), data); }

        private:
        std::array<Word, pow3(io_address_width)> io_space;
        std::map<int, read_handler_t> read_handlers;
        std::map<int, write_handler_t> write_handlers;
    };
}

#endif /* TRIREME_IO_HPP */