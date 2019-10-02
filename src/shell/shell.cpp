#include "shell/shell.hpp"

using namespace ternary::shell;
namespace algo = boost::algorithm;

Shell::Shell(Cpu& cpu): cpu_(cpu), repl_(), asm_(), ip_(cpu_.get_instruction_pointer())
{
    cpu_.reset();


}

void Shell::start()
{
    // Labels can't be on a line by themselves, so we need to build them up.
    std::string multiline_input {};

    for (;;)
    {
        const char* cinput { nullptr };
        do
        {
            cinput = repl_.input(prompt(multiline_input));
        } while ((cinput == nullptr) && (errno == EAGAIN));

        if (cinput == nullptr)
        {
            break;
        }

        std::string input { cinput };

        // Multline handling hack
        if (algo::ends_with(algo::trim_copy(input), ":"))
        {
            multiline_input.append(input);
            continue;
        }
        if (!multiline_input.empty())
        {
            input = multiline_input + input;
            multiline_input.clear();
        }

        // General handling
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
            if (input.front() == '.')
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

std::string Shell::prompt(const std::string& continuation)
{
    auto actual_ip { cpu_.get_instruction_pointer() };

    auto cont_prompt { ""s };

    if (!continuation.empty())
    {
        cont_prompt = fmt::format(" \x1b[1;33m{0}\x1b[0m", continuation.c_str());
    }

    if (ip_ == actual_ip)
    {
        return fmt::format("\x1b[1;32m{0}\x1b[0m{1} > ", ip_, cont_prompt);
    }
    else
    {
        return fmt::format("\x1b[1;92m{0}\x1b[0m \x1b[32m(IP {1})\x1b[0m{2} > ", ip_, actual_ip, cont_prompt);
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