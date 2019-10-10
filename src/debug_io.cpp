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

    void DebugIo::write(int input)
    {
        switch (mode)
        {
            case 0:
            {
                if (input)
                {
                    backing_output.push_back(static_cast<unsigned char>(input));
                }
                else
                {
                    print_and_clear();
                }
                break;
            }
            case 1:
            {
                backing_output = std::to_string(input);
                print_and_clear();
                break;
            }
            case -1:
            {
                Word w { input };
                backing_output = w.value_string();
                print_and_clear();
                break;
            }
            default:
                break;
        }

    }

    int DebugIo::read_control()
    {
        return mode * 3 + request;
    }

    void DebugIo::write_control(int value)
    {
        request = low_trits(value, 1);

        mode = shift_right(low_trits(value, 2), 1);
    }
}