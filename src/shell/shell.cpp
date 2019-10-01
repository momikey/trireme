#include "shell/shell.hpp"

using namespace ternary::shell;

Shell::Shell(Cpu& cpu): cpu_(cpu), repl_(), asm_(), ip_(cpu_.get_instruction_pointer())
{
    cpu_.reset();


}

void Shell::start()
{
    // Labels can't be on a line by themselves, so we need to build them up.
    std::string multiline_container {};

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
            if (input[0] == '.')
            {
                handle_command(input.substr(1));
            }
            else
            {
                assemble_instruction(input);
            }
        }
    }
}

std::string Shell::prompt()
{
    auto actual_ip { cpu_.get_instruction_pointer() };

    if (ip_ == actual_ip)
    {
        return fmt::format("\x1b[1;32m{0}\x1b[0m > ", ip_);
    }
    else
    {
        return fmt::format("\x1b[1;92m{0}\x1b[0m \x1b[32m(IP {1})\x1b[0m > ", ip_, actual_ip);
    }
}

void Shell::assemble_instruction(const std::string& input)
{
    auto result { asm_.assemble_line(input, ip_.value()) };

    if (!result.empty())
    {
        cpu_.load(result);

        // TODO: Handle possibility of out-of-order instruction assembly?
        auto new_ip { add(ip_, result.size()).first };
        if (new_ip.high().get())
        {
            // TODO: Handle advanced architecture
            new_ip.set_high(0);
        }
        ip_.set(new_ip);
    }
}

void Shell::handle_command(const std::string& input)
{
    repl_.print("%s\n", fmt::format("{0}: {1}", ip_, input).c_str());
}