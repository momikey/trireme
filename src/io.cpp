#include "io.hpp"

#include "word.hpp"
#include "ternary_math.hpp"

namespace ternary
{
    Word Io::read(int addresss)
    {
        if (read_handlers[address])
        {
            return read_handlers[address]();
        }
        else
        {
            return io_space[address];
        }
    }

    void Io::write(int address, int data)
    {
        if (write_handlers[address])
        {
            write_handlers[address](data);
        }
        else
        {
            io_space[address] = data;
        }
    }

    // TODO: Base conversion?
    int Io::read_binary(int address)
    {
        if (read_handlers[address])
        {
            return read_handlers[address]();
        }
        else
        {
            return io_space[address];
        }
    }

    void Io::write_binary(int address, unsigned char data)
    {
        if (write_handlers[address]) {
            write_handlers[address](data);
        }
        else
        {
            io_space[address] = data;
        }
    }
}
