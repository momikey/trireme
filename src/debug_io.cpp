#include "debug_io.hpp"

namespace ternary
{
    char DebugIo::read()
    {
        if (request == 1)
        {
            backing_input.clear();
            std::cout << "> ";
            get_input();
            
            request = 0;
        }

        if (backing_input.empty())
        {
            return 0;
        }
        else
        {
            char c { backing_input[0] };
            backing_input.erase(backing_input.begin());
            return c;
        }
    }

    void DebugIo::write(char c)
    {
        if (c)
        {
            backing_output.push_back(c);
        }
        else
        {
            print();
            backing_output.clear();
        }
    }

    int DebugIo::read_control()
    {
        return request;
    }

    void DebugIo::write_control(int value)
    {
        request = low_trits(value, 1);
    }
}