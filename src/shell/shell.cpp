#include "shell/shell.hpp"

using namespace std::string_literals;

using namespace std::placeholders;

using namespace ternary::shell;
namespace algo = boost::algorithm;
using replxx::Replxx;

Shell::Shell(Cpu& cpu): cpu_(cpu), repl_(), asm_(), ip_(cpu_.get_instruction_pointer())
{
    cpu_.reset();

    repl_.set_max_history_size(100);

    ////
    // Insert keybindings. These are pretty basic for now:
    // * Ctrl-C: Cancel line)
    // * Ctrl-W: Delete word
    // * Up: Previous in history list
    // * Down: Next in history list
    ////
    repl_.bind_key(Replxx::KEY::control('c'),
        std::bind(&Replxx::invoke, &repl_, Replxx::ACTION::ABORT_LINE, _1));
    repl_.bind_key(Replxx::KEY::control('d'),
        std::bind(&Replxx::invoke, &repl_, Replxx::ACTION::SEND_EOF, _1));
    repl_.bind_key(Replxx::KEY::control('w'),
        std::bind(&Replxx::invoke, &repl_, Replxx::ACTION::KILL_TO_BEGINING_OF_WORD, _1));

    repl_.bind_key(Replxx::KEY::UP,
        std::bind(&Replxx::invoke, &repl_, Replxx::ACTION::HISTORY_PREVIOUS, _1));
    repl_.bind_key(Replxx::KEY::DOWN,
        std::bind(&Replxx::invoke, &repl_, Replxx::ACTION::HISTORY_NEXT, _1));

    ////
    // Insert shell commands
    ////
    commands["reg"] = std::bind(&Shell::handle_register, this, _1);
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
            // This means we got an EOF, so exit gracefully with a newline
            repl_.print("\n");
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
        else if (input == ".quit"s || input == ".exit"s)
        {
            break;
        }
        else
        {
            if (input.front() == '.')
            {
                handle_command(algo::trim_copy(input.substr(1)));
            }
            else
            {
                assemble_instruction(input);
            }

            repl_.history_add(input);
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
    std::regex command { R"(^(\w+)(?:\s+(.*))?)" };
    std::smatch matches;

    auto word_to_value = [] (std::string raw) {
        return (raw.front() != '%') ? std::stoi(raw) : string_to_value(raw.substr(1));
    };

    if (std::regex_match(input, matches, command))
    {
        if (matches[1] == "reg")
        {
            ////
            // Get or set a register
            ////

            // Try to match a register and (optionally) a setter
            std::smatch reg_matches;
            std::string rest { matches[2].str() };
            if (std::regex_match(rest, reg_matches, match_register))
            {
                const auto reg_number { string_to_value(reg_matches[1].str()) };

                if (reg_matches[2].str().empty())
                {
                    // Get register
                    auto result { cpu_.get_register(reg_number) };

                    repl_.print("r%s: %s\n", reg_matches[1].str().c_str(),
                        fmt::format("\x1b[1m{0}\x1b[0m ({1})", result, result.value()).c_str());
                }
                else
                {
                    // Set register
                    const auto raw_value { reg_matches[2].str() };
                    // auto value = (raw_value[0] != '%') ?
                    //     std::stoi(raw_value) : string_to_value(raw_value.substr(1));
                    auto value = word_to_value(raw_value);
                    
                    cpu_.set_reg(reg_number, value);
                }
            }
            else
            {
                repl_.print("Invalid format\n");
            }
        }
        else if (matches[1] == "mem" || matches[1] == "memory")
        {
            ////
            // Get or set memory
            ////

            std::smatch mem_matches;
            std::string rest { matches[2].str() };
            if (std::regex_match(rest, mem_matches, match_memory))
            {
                if (mem_matches[2].str().empty())
                {
                    // Get memory
                    auto result { cpu_.get_memory_word(word_to_value(mem_matches[1].str())) };

                    repl_.print("Mem %s: %s\n", mem_matches[1].str().c_str(),
                        fmt::format("\x1b[1m{0}\x1b[0m ({1})", result, result.value()).c_str());
                }
                else
                {
                    auto address = word_to_value(mem_matches[1].str());
                    auto value = word_to_value(mem_matches[2].str());

                    cpu_.set_memory_word(address, value);
                }   
            }
            else
            {
                repl_.print("Invalid format\n");
            }
        }
        else if (matches[1] == 'assemble' || matches[1] == 'asm')
        {
            ////
            // Assemble a file and load into memory
            ////

            std::string filename { matches[2].str() };

            
        }
    }

    repl_.print("%s\n", fmt::format("{0}: {1}", ip_, input).c_str());
}

bool Shell::handle_register(const std::string& rest)
{
    repl_.print("%s\n", rest.c_str());
    return true;
}