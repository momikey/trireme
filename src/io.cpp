#include "io.hpp"

#include "word.hpp"
#include "ternary_math.hpp"

namespace ternary
{
    Word Io::read(int address)
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
            return io_space[address].value();
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

    bool Io::bind(int port, read_handler_t reader)
    {
        if (read_handlers[port])
        {
            return false;
        }
        else
        {
            read_handlers[port] = reader;
            return true;
        }
    }

    bool Io::bind(int port, write_handler_t writer)
    {
        if (write_handlers[port])
        {
            return false;
        }
        else
        {
            write_handlers[port] = writer;
            return true;
        }
    }

    void Io::unbind(int port, RW which)
    {
        switch (which)
        {
            case RW::read:
                read_handlers.erase(port);
                break;

            case RW::write:
                write_handlers.erase(port);
                break;
        }
    }
}
