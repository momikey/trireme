#include "shell/shell.hpp"

using namespace ternary::shell;

Shell::Shell(Cpu& cpu): cpu_(cpu), repl_(), asm_()
{
    cpu_.reset();


}

void Shell::start()
{
    for (;;)
    {
        const char* cinput { nullptr };
        do
        {
            cinput = repl_.input(prompt());
        } while ((cinput == nullptr) && (errno == EAGAIN));

        if (cinput == nullptr)
        {
            break;
        }

        std::string input { cinput };

        if (input.empty())
        {
            continue;
        }
        else if (input == ".quit"s)
        {
            break;
        }
        else
        {
            repl_.print("%s\n", fmt::format("{0}: {1}", cpu_.get_instruction_pointer(), input).c_str());
        }
    }
}

std::string Shell::prompt()
{
    return fmt::format("Trireme \x1b[1;32m{0}\x1b[0m >", cpu_.get_instruction_pointer());
}