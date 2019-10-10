#include "cpu.hpp"

namespace ternary
{
    Cpu::Cpu()
    {
        using std::placeholders::_1;

        Io::read_handler_t read { std::bind(&DebugIo::read, &debug_io) };
        Io::write_handler_t write { std::bind(&DebugIo::write, &debug_io, _1) };
        
        Io::read_handler_t read_control { std::bind(&DebugIo::read_control, &debug_io) };
        Io::write_handler_t write_control { std::bind(&DebugIo::write_control, &debug_io, _1) };

        io.bind(debug_io_base - 2, write);
        io.bind(debug_io_base - 1, read);

        io.bind(debug_io_base, read_control);
        io.bind(debug_io_base, write_control);
    }

    /**
     * @brief Reset the simulation to the "boot" state. This does not clear memory,
     * but it does set all registers to their initial values.
     */
    void Cpu::reset()
    {
        registers.clear_all();
        std::fill(control_regs.begin(), control_regs.end(), 0);
        std::fill(debug_regs.begin(), debug_regs.end(), 0);
        flag_register = {};

        instruction_pointer.set(0, Hexad::min_value, Hexad::min_value);
        control_regs[2].set(0, -280, -1);
        registers.set(-9, { 0, -283, -364});
    }

    /**
     * @brief Clear the simulated CPU's memory.
     * 
     */
    void Cpu::clear_memory()
    {
        memory.clear();
    }

    /**
     * @brief Load data from a map of address/value pairs. This doesn't do
     * much in the way of error-handling yet.
     * 
     * @param data A structure mapping addresses to hexad values
     */
    void Cpu::load(std::map<int, Hexad> data)
    {
        for (auto& pair : data)
        {
            memory.set(pair.first, pair.second);
        }
    }

    /**
     * @brief Start the simulated CPU. This begins execution at
     * address %00zzzz and runs until an unhandled exception or
     * a special debug I/O write.
     * 
     */
    void Cpu::run()
    {
        // TODO
        reset();

        while (true)
        {
            auto breakpoint { step() };

            if (breakpoint)
            {
                break;
            }
        }
    }

    /**
     * @brief Execute a single instruction on the simulated CPU.
     * 
     */
    bool Cpu::step()
    {
        // TODO
        auto current_ip { instruction_pointer };

        bool breakpoint_encountered { false };

        // We wrap the main fetch/decode loop in a try/catch
        // for easier handling of interrupts (since we represent
        // them as exceptions.)
        try
        {
            Opcode instruction { memory.get_word(current_ip.value()) };
            decode_major(instruction);
            
            // If debug breakpoints are enabled, check to see whether we
            // have reached one. If so, raise the interrupt.
            if (flag_register.get_flag(flags::trap) &&
                std::find(debug_regs.cbegin(), debug_regs.cend(), current_ip) != debug_regs.cend()
            )
            {
                throw debug_breakpoint{};
            }
        }
        catch (const system_interrupt_base& e)
        {
            // Architecture interrupts are all derived from the
            // `system_interrupt_base` class, and they all have
            // a `value()` method that gives their interrupt #.
            
            // The hardware interrupt vector table is stored in
            // the CR2 register. We take the interrupt # as an
            // index into this table.
            Word interrupt_vector { add(control_regs[2], e.value()*3).first };
            Word interrupt_address { get_memory_word(interrupt_vector.value()) };
            
            // Save the current IP into CR3
            control_regs[3].set(instruction_pointer);

            // Now jump to the interrupt handler
            instruction_pointer.set(interrupt_address);

            // For a debug breakpoint, set the return flag
            if (e.value() == 1)
            {
                breakpoint_encountered = true;
            }
        }
        catch (const std::exception& e)
        {
            // Other exceptions are, well, exceptional
            std::cerr << e.what() << '\n';
            throw;
        }
        
        // Branch, call, return, and syscall/sysret will all change IP.
        // If they don't, then we increment it by 3 (instructions are
        // word-aligned on our architecture.)
        if (instruction_pointer.value() == current_ip.value())
        {
            instruction_pointer.set(add(current_ip.value(), 3).first);
        }

        return breakpoint_encountered;
    }

    Hexad Cpu::read_memory(const int ad)
    {
        auto f { flag_register.get_flag(flags::absolute) };

        switch (f)
        {
            case 0:
                return memory.get(ad);
            
            case 1:
                return memory.get(ad + registers.get(-3).value());
            
            default:
                throw invalid_flag{};
        }
    }

    Word Cpu::read_memory_word(const int ad)
    {
        auto f { flag_register.get_flag(flags::absolute) };

        switch (f)
        {
            case 0:
                return memory.get_word(ad);
            
            case 1:
                return memory.get_word(ad + registers.get(-3).value());
            
            default:
                throw invalid_flag{};
        }
    }

    void Cpu::write_memory(const int ad, const int val)
    {
        auto f { flag_register.get_flag(flags::absolute) };

        switch (f)
        {
            case 0:
                return memory.set(ad, val);
            
            case 1:
                return memory.set(ad + registers.get(-3).value(), val);
            
            default:
                throw invalid_flag{};
        }
    }

    void Cpu::write_memory_word(const int ad, const int val)
    {
        auto f { flag_register.get_flag(flags::absolute) };

        switch (f)
        {
            case 0:
                return memory.set_word(ad, val);
            
            case 1:
                return memory.set_word(ad + registers.get(-3).value(), val);
            
            default:
                throw invalid_flag{};
        }
    }

    void Cpu::decode_major(Opcode& op)
    {
        switch (op.o)
        {
            case 0:
                decode_minor_system(op);
                break;
            case 1:
                decode_minor_tritwise(op);
                break;
            case 2:
                decode_minor_complex(op);
                break;
            case 3:
                // %C.... is undefined on the basic architecture
                undefined_opcode(op);
                break;
            case 4:
                decode_minor_arithmetic(op);
                break;
            case 5:
                load_register_memory(op.m, op.low12(), hexad_select::low);
                break;
            case 6:
                load_register_memory(op.m, op.low12(), hexad_select::middle);
                break;
            case 7:
                load_register_memory(op.m, op.low12(), hexad_select::high);
                break;
            case 8:
                decode_minor_register(op);
                break;
            case 9:
                load_register_memory(op.m, op.low12(), hexad_select::full_word);
                break;
            case 10:
                decode_minor_branch(op);
                break;
            case 11:
                branch_on_flag(op.low12(), op.m, -1);
                break;
            case 12:
                branch_on_flag(op.low12(), op.m, 0);
                break;
            case 13:
                branch_on_flag(op.low12(), op.m, 1);
                break;
            case -1:
                undefined_opcode(op);
                break;
            case -2:
                undefined_opcode(op);
                break;
            case -3:
                // %p..... is undefined on the basic architecture
                undefined_opcode(op);
                break;
            case -4:
                undefined_opcode(op);
                break;
            case -5:
                undefined_opcode(op);
                break;
            case -6:
                decode_minor_set(op);
                break;
            case -7:
                undefined_opcode(op);
                break;
            case -8:
                decode_minor_io(op);
                break;
            case -9:
                store_register_memory(op.m, op.low12(), hexad_select::full_word);
                break;
            case -10:
                decode_minor_indirect(op);
                break;
            case -11:
                store_register_memory(op.m, op.low12(), hexad_select::high);
                break;
            case -12:
                store_register_memory(op.m, op.low12(), hexad_select::middle);
                break;
            case -13:
                store_register_memory(op.m, op.low12(), hexad_select::low);
                break;

            default:
                undefined_opcode(op);
                break;
        }
    }

    void Cpu::decode_minor_system(Opcode& op)
    {
        switch (op.m)
        {
            case 0:
                // %00.... is an *intentional* undefined opcode
                undefined_opcode(op);
                break;
            case 1:
                system_load_register(op.x, op.z);
                break;
            case 10:
                system_call(op.low6());
                break;
            case -1:
                system_store_register(op.x, op.z);
            case -11:
                // %0x.... is a NOP
                break;
            case -12:
                system_breakpoint();
                break;
            case -13:
                system_return();
            default:
                undefined_opcode(op);
                break;
        }
    }

    void Cpu::decode_minor_tritwise(Opcode& op)
    {
        switch (op.m)
        {
            case 0:
                invert_register(op.z, sti);
                break;
            case 1:
                invert_register(op.z, pti);
                break;
            case 8:
                logical_register(op.x, op.y, op.z, min);
                break;
            case 9:
                logical_register(op.x, op.y, op.z, teq);
                break;
            case 10:
                logical_register(op.x, op.y, op.z, max);
                break;
            case 11:
                logical_register(op.x, op.y, op.z, tem);
                break;
            case -1:
                invert_register(op.z, nti);
                break;
            case -5:
                rotate_register(op.x, op.low6(), false);
                break;
            case -7:
                rotate_register(op.x, op.low6(), true);
                break;
            case -8:
                compare_immediate(op.x, op.low6());
                break;
            case -9:
                compare_register(op.y, op.z);
                break;
            case -10:
                rotate_register_carry(op.x, op.low6(), true);
                break;
            case -11:
                shift_register(op.x, op.low6(), false);
                break;
            case -12:
                rotate_register_carry(op.x, op.low6(), false);
                break;
            case -13:
                shift_register(op.x, op.low6(), true);
                break;
            default:
                undefined_opcode(op);
                break;
        }
    }

    void Cpu::decode_minor_arithmetic(Opcode& op)
    {
        switch (op.m)
        {
            case 9:
                add_subtract_register(op.x, op.y, op.z, false);
                break;
            case 11:
                add_subtract_immediate(op.t, op.t, op.low9(), false);
                break;
            case 12:
                add_subtract_carry(op.x, op.y, op.z, false);
                break;
            case 13:
                add_subtract_immediate(op.t, op.x, op.low6(), false);
                break;
            case -6:
                add_subtract_immediate(op.t, op.x, op.low6(), true);
                break;
            case -8:
                add_subtract_immediate(op.t, op.t, op.low9(), true);
                break;
            case -9:
                add_subtract_register(op.x, op.y, op.z, true);
                break;
            case -10:
                add_subtract_carry(op.x, op.y, op.z, true);
                break;
            default:
                undefined_opcode(op);
                break;
        }
    }

    void Cpu::decode_minor_complex(Opcode& op)
    {
        switch (op.m)
        {
            case 1:
                register_conversion(op.y, op.z, bin);
                break;
            case 2:
                register_conversion(op.y, op.z, rdr);
                break;
            case 4:
                register_conversion(op.y, op.z, fdr);
                break;
            case 9:
                divide_register(op.x, op.y, op.z);
                break;
            case 12:
                multiply_register(op.x, op.y, op.z);
                break;
            case -1:
                register_conversion(op.y, op.z, tri);
                break;
            case -6:
                multiply_immediate(op.t, op.x, op.low6());
                break;
            case -7:
                multiply_immediate(op.t, op.t, op.low9());
                break;
            case -9:
                divide_immediate(op.t, op.x, op.low6());
                break;
            case -10:
                divide_immediate(op.t, op.t, op.low9());
                break;
            default:
                undefined_opcode(op);
                break;
        }
    }

    void Cpu::decode_minor_register(Opcode& op)
    {
        switch (op.m)
        {
            case 0:
                load_register_immediate(op.x, op.low6(), hexad_select::full_word);
                break;
            case 1:
                decode_tertiary_HA(op);
                break;
            case 2:
                load_register_immediate(op.x, op.low6(), hexad_select::low);
                break;
            case 3:
                load_register_immediate(op.x, op.low6(), hexad_select::middle);
                break;
            case 4:
                load_register_immediate(op.x, op.low6(), hexad_select::high);
                break;
            case 5:
                load_register_indirect(op.y, op.z, hexad_select::low);
                break;
            case 6:
                load_register_indirect(op.y, op.z, hexad_select::middle);
                break;
            case 7:
                load_register_indirect(op.y, op.z, hexad_select::high);
                break;
            case 8:
                load_register_address(op.low12());
                break;
            case 9:
                load_register_indirect(op.y, op.z, hexad_select::full_word);
                break;
            case 11:
                push_register(op.z);
                break;
            case 13:
                pop_register(op.z);
                break;
            case -1:
                // Note swapped order!!!
                move_register(op.z, op.y);
                break;
            case -4:
                exchange_registers(op.y, op.z);
                break;
            default:
                undefined_opcode(op);
                break;
        }
    }

    void Cpu::decode_minor_branch(Opcode& op)
    {
        switch (op.m)
        {
            case 0:
                branch_call(op.low12(), false);
                break;
            case 1:
                branch_relative(op.low12());
                break;
            case 2:
                branch_register(op.x, op.low6(), false);
                break;
            case 3:
                branch_register(op.x, 0, false);
                break;
            case 4:
                branch_absolute(op.low12());
                break;
            case 11:
                branch_on_flag(3, op.z, -1);
                break;
            case 12:
                branch_on_flag(3, op.z, 0);
                break;
            case 13:
                branch_on_flag(3, op.z, 1);
                break;
            case -1:
                branch_relative(op.low6());
                break;
            case -2:
                // Note different order of operands!!!
                branch_ternary(static_cast<flags>(op.t), op.z, op.y, op.x);
                break;
            case -3:
                branch_register(op.z, 0, true);
                break;
            case -9:
                branch_call(op.low12(), true);
                break;
            case -13:
                branch_return();
                break;
            default:
                undefined_opcode(op);
                break;
        }
    }

    void Cpu::decode_minor_indirect(Opcode& op)
    {
        switch (op.m)
        {
            case 5:
                store_register_indirect(op.y, op.z, hexad_select::low);
                break;
            case 6:
                store_register_indirect(op.y, op.z, hexad_select::middle);
                break;
            case 7:
                store_register_indirect(op.y, op.z, hexad_select::high);
                break;
            case 9:
                store_register_indirect(op.y, op.z, hexad_select::full_word);
                break;
            default:
                undefined_opcode(op);
                break;
        }
    }

    void Cpu::decode_minor_set(Opcode& op)
    {
        switch (op.m)
        {
            case 0:
                set_flag_to_value(static_cast<flags>(op.z), 0);
                break;
            case 1:
                set_flag_to_value(static_cast<flags>(op.z), 1);
                break;
            case -1:
                set_flag_to_value(static_cast<flags>(op.z), -1);
            default:
                undefined_opcode(op);
                break;
        }
    }

    void Cpu::decode_tertiary_HA(Opcode& op)
    {
        switch (op.t)
        {
            case 0:
                set_register(op.y, 0);
                break;
            case 1:
                set_register(op.y, 1);
                break;
            case -1:
                set_register(op.y, -1);
                break;
            default:
                undefined_opcode(op);
                break;
        }
    }

    void Cpu::decode_minor_io(Opcode& op)
    {
        switch (op.m)
        {
            case 1:
                io_read(op.t, op.low9(), false);
                break;
            case 2:
                io_write(op.t, op.low9(), true);
                break;
            case 4:
                io_read(op.t, op.low9(), true);
                break;
            case -1:
                io_write(op.t, op.low9(), false);
                break;
            default:
                undefined_opcode(op);
                break;
        }
    }

    void Cpu::undefined_opcode(Opcode& op)
    {
        throw invalid_opcode{};
    }

    void Cpu::load_register_immediate(const int reg, const int value, hexad_select type)
    {
        if (type == hexad_select::full_word)
        {
            Word w { 0, 0, value };
            registers.set(reg, w);
        }
        else
        {
            Hexad h { value };
            Word current { registers.get(reg) };

            switch (type)
            {
                case hexad_select::low:
                    current.set_low(h);
                    break;

                case hexad_select::middle:
                    current.set_middle(h);
                    break;
                
                case hexad_select::high:
                    current.set_high(h);
                    break;
            
                default:
                    assert(0);
                    break;
            }

            registers.set(reg, current);
        }
    }

    void Cpu::load_register_memory(const int reg, const int addr, hexad_select type)
    {
        if (type == hexad_select::full_word)
        {
            Word w { read_memory_word(addr) };
            registers.set(reg, w);

            flag_register.set_flag(flags::sign, sign_c(w.value()));
        }
        else
        {
            Hexad h { read_memory(addr) };
            Word current { registers.get(reg) };

            switch (type)
            {
                case hexad_select::low:
                    current.set_low(h);
                    break;

                case hexad_select::middle:
                    current.set_middle(h);
                    break;
                
                case hexad_select::high:
                    current.set_high(h);
                    break;
            
                default:
                    assert(0);
                    break;
            }

            registers.set(reg, current);

            flag_register.set_flag(flags::sign, sign_c(current.value()));
        }
    }

    void Cpu::load_register_indirect(const int addrreg, const int destreg, hexad_select type)
    {
        auto address { registers.get(addrreg) };
        auto current { registers.get(destreg) };

        // If the A flag is set +, use it as a base
        if (flag_register.get_flag(flags::absolute) == 1)
        {
            address = add(address, registers.get(-3)).first;
        }

        // On the basic architecture, clear high hexad
        // TODO: Implement advanced architecture w/18-trit address space
        address.set_high(0);

        if (type == hexad_select::full_word)
        {
            current.set(read_memory_word(address.value()));
        }
        else
        {
            auto data { read_memory(address.value()) };

            switch (type)
            {
            case hexad_select::low:
                current.set_low(data);
                break;
            case hexad_select::middle:
                current.set_middle(data);
                break;
            case hexad_select::high:
                current.set_high(data);
                break;
            default:
                break;
            }

        }

        registers.set(destreg, current);

        flag_register.set_flag(flags::sign, sign_c(current.value()));
    }

    void Cpu::load_register_indexed(const int destreg, const int addr)
    {
        auto address { registers.get(-3).value() + registers.get(-1).value() + addr };

        registers.set(destreg, read_memory_word(address));
        
        if (flag_register.get_flag(flags::direction))
        {
            registers.set(destreg, add(address, 3).first);
        }

        flag_register.set_flag(flags::sign, sign_c(registers.get(destreg).value()));
    }

    void Cpu::load_register_address(const int addr)
    {
        Word w;

        if (flag_register.get_flag(flags::absolute))
        {
            w.set(add(registers.get(-3), addr).first);
        }
        else
        {
            w = addr;
        }

        w.set_high(0);

        registers.set(0, w);        
    }

    void Cpu::store_register_memory(const int reg, const int addr, hexad_select type)
    {
        Word r { registers.get(reg) };
        if (type == hexad_select::full_word)
        {
            write_memory_word(addr, r);
        }
        else
        {
            switch (type)
            {
                 case hexad_select::low:
                    write_memory(addr, r.low());
                    break;

                case hexad_select::middle:
                    write_memory(addr, r.middle());
                    break;
                
                case hexad_select::high:
                    write_memory(addr, r.high());
                    break;
            
                default:
                    assert(0);
                    break;               
            }
        }

        flag_register.set_flag(flags::sign, sign_c(r.value()));
    }

    void Cpu::store_register_indirect(const int srcreg, const int addrreg, hexad_select type)
    {
        auto address { registers.get(addrreg) };
        auto current { registers.get(srcreg) };

        // If the A flag is set +, use it as a base
        if (flag_register.get_flag(flags::absolute) == 1)
        {
            address = add(address, registers.get(-3)).first;
        }

        // On the basic architecture, clear high hexad
        // TODO: Implement advanced architecture w/18-trit address space
        address.set_high(0);

        if (type == hexad_select::full_word)
        {
            write_memory_word(address.value(), current);
        }
        else
        {
            switch (type)
            {
                 case hexad_select::low:
                    write_memory(address.value(), current.low().get());
                    break;

                case hexad_select::middle:
                    write_memory(address.value(), current.middle().get());
                    break;
                
                case hexad_select::high:
                    write_memory(address.value(), current.high().get());
                    break;
            
                default:
                    assert(0);
                    break;               
            }
        }

        flag_register.set_flag(flags::sign, sign_c(current.value()));
    }

    void Cpu::store_register_indexed(const int srcreg, const int addr)
    {
        auto address { registers.get(-3).value() + registers.get(-1).value() + addr };

        write_memory_word(address, registers.get(srcreg));

        if (flag_register.get_flag(flags::direction))
        {
            registers.set(srcreg, add(address, 3).first);
        }

        flag_register.set_flag(flags::sign, sign_c(registers.get(srcreg).value()));
    }

    void Cpu::push_register(const int reg)
    {
        // rs = stack pointer
        auto sp { registers.get(-6) };
        auto data { registers.get(reg) };

        memory.set_word(sp.value(), data);

        auto newsp  { sub(sp, 3) };

        // TODO: Handle stack overflow, probably using carry from the
        // previous operation.
        registers.set(-6, newsp.first);

        flag_register.set_flag(flags::sign, sign_c(data.value()));
    }

    void Cpu::pop_register(const int reg)
    {
        // rs = stack pointer
        auto sp { registers.get(-6) };

        auto newsp  { add(sp, 3) };

        // TODO: Handle stack overflow, probably using carry from the
        // previous operation.
        registers.set(-6, newsp.first);

        auto data { memory.get_word(newsp.first.value()) };

        registers.set(reg, data);

        flag_register.set_flag(flags::sign, sign_c(data.value()));
    }

    void Cpu::branch_on_flag(const int addr, const int flag, const int target)
    {
        if (flag_register.get_flag(static_cast<flags>(flag)) == target)
        {
            auto newaddr { add(instruction_pointer, addr).first };
            instruction_pointer.set(newaddr);
        }
    }

    void Cpu::add_subtract_register(const int srcreg1, const int srcreg2, const int destreg, const bool subtract)
    {
        auto src1 { registers.get(srcreg1) };
        auto src2 { registers.get(srcreg2) };

        auto result { subtract ? sub(src1, src2) : add(src1, src2) };

        registers.set(destreg, result.first);
        flag_register.set_flag(flags::carry, result.second);
        flag_register.set_flag(flags::sign, sign_c(result.first.value()));
    }

    void Cpu::add_subtract_carry(const int srcreg1, const int srcreg2, const int destreg, const bool subtract)
    {
        auto src1 { registers.get(srcreg1) };
        auto src2 { registers.get(srcreg2) };
        auto carry { flag_register.get_flag(flags::carry) };

        auto result { subtract ? sub(src1, src2) : add(src1, src2) };
        auto with_carry { subtract ? sub(result.first, carry) : add(result.first, carry) };
        auto final_carry { result.second + with_carry.second };

        registers.set(destreg, with_carry.first);
        flag_register.set_flag(flags::carry, final_carry);
        flag_register.set_flag(flags::sign, sign_c(with_carry.first.value()));
    }

    void Cpu::add_subtract_immediate(const int srcreg, const int destreg, const int immediate, const bool subtract)
    {
        auto src { registers.get(srcreg) };

        auto result { subtract ? sub(src, immediate) : add(src, immediate) };

        registers.set(destreg, result.first);
        flag_register.set_flag(flags::carry, result.second);
        flag_register.set_flag(flags::sign, sign_c(result.first.value()));
    }

    void Cpu::multiply_register(const int srcreg1, const int srcreg2, const int destreg)
    {
        auto src1 { registers.get(srcreg1) };
        auto src2 { registers.get(srcreg2) };

        auto result { mul(src1, src2) };

        registers.set(destreg, result.first);
        // ro = high word of result
        registers.set(-2, result.second);

        flag_register.set_flag(flags::carry, sign_c(result.second.value()));
        flag_register.set_flag(flags::sign, result.second.value() ? 
            sign_c(result.second.value()) : sign_c(result.first.value()));
    }

    void Cpu::multiply_immediate(const int srcreg, const int destreg, const int immediate)
    {
        auto src { registers.get(srcreg) };
        auto result { mul(src, immediate) };

        registers.set(destreg, result.first);
        // ro = high word of result
        registers.set(-2, result.second);

        flag_register.set_flag(flags::carry, sign_c(result.second.value()));
        flag_register.set_flag(flags::sign, result.second.value() ? 
            sign_c(result.second.value()) : sign_c(result.first.value()));
    }

    void Cpu::divide_register(const int srcreg1, const int srcreg2, const int destreg)
    {
        if (registers.get(srcreg2).value())
        {
            auto src1 { registers.get(srcreg1) };
            auto src2 { registers.get(srcreg2) };

            // ro = high word of dividend
            auto highword { registers.get(-2) };
            division_dividend di { highword, src1 };

            auto result { div(di, src2) };

            registers.set(destreg, result.first.value());
            // ru = remainder
            registers.set(-8, result.second.value());

            // Check for overflow
            if (result.first.value() * src2.value() + result.second.value() !=
                highword.value() * static_cast<long long int>(pow3(Word::word_size)) + src2.value())
            {
                flag_register.set_flag(flags::carry, highword.value() ?
                    sign_c(highword.value()) : sign_c(src2.value()));
            }
            else
            {
                flag_register.set_flag(flags::carry, 0);
            }

            flag_register.set_flag(flags::sign, sign_c(result.first.value()));
        }
        else
        {
            throw divide_by_zero{};
        }
        
    }

    void Cpu::divide_immediate(const int srcreg, const int destreg, const int immediate)
    {
        if (immediate)
        {
            auto src { registers.get(srcreg) };

            // ro = high word of dividend
            auto highword { registers.get(-2) };
            division_dividend di { highword, src };

            auto result { div(di, immediate) };

            registers.set(destreg, result.first.value());
            // ru = remainder
            registers.set(-8, result.second.value());

            // Check for overflow
            if (result.first.value() * immediate + result.second.value() !=
                highword.value() * static_cast<long long int>(pow3(Word::word_size)) + immediate)
            {
                flag_register.set_flag(flags::carry, highword.value() ?
                    sign_c(highword.value()) : sign_c(immediate));
            }
            else
            {
                flag_register.set_flag(flags::carry, 0);
            }

            flag_register.set_flag(flags::sign, sign_c(result.first.value()));
        }
        else
        {
            throw divide_by_zero{};
        }        
    }

    void Cpu::register_conversion(const int srcreg, const int destreg, unary_function fun)
    {
        auto src { registers.get(srcreg) };

        registers.set(destreg, fun(src));

        // conversions don't affect flags
    }

    void Cpu::invert_register(const int reg, unary_function fun)
    {
        auto src { registers.get(reg) };

        registers.set(reg, fun(src));

        flag_register.set_flag(flags::sign, sign_c(registers.get(reg).value()));
    }

    void Cpu::logical_register(const int srcreg1, const int srcreg2, const int destreg, binary_function fun)
    {
        auto src1 { registers.get(srcreg1) };
        auto src2 { registers.get(srcreg2) };

        auto result { fun(src1, src2) };

        registers.set(destreg, result);

        flag_register.set_flag(flags::sign, sign_c(result.value()));
    }

    void Cpu::compare_register(const int lreg, const int rreg)
    {
        auto lhs { registers.get(lreg) };
        auto rhs { registers.get(rreg) };

        auto result { sub(lhs, rhs) };

        flag_register.set_flag(flags::carry, result.second);
        flag_register.set_flag(flags::sign, sign_c(result.first.value()));
    }

    void Cpu::compare_immediate(const int reg, const int immediate)
    {
        auto lhs { registers.get(reg) };

        auto result { sub(lhs, immediate) };

        flag_register.set_flag(flags::carry, result.second);
        flag_register.set_flag(flags::sign, sign_c(result.first.value()));
    }

    void Cpu::shift_register(const int reg, const int places, bool right)
    {
        auto value { registers.get(reg) };

        auto result { right ? shr(value, low_trits(places, 4)) : shl(value, low_trits(places, 4)) };

        registers.set(reg, result.first);

        flag_register.set_flag(flags::carry, result.second);
        flag_register.set_flag(flags::sign, sign_c(result.first.value()));
    }

    void Cpu::rotate_register(const int reg, const int places, bool right)
    {
        auto value { registers.get(reg) };

        auto result { right ? ror(value, low_trits(places, 4)) : rol(value, low_trits(places, 4)) };

        registers.set(reg, result.first);

        flag_register.set_flag(flags::sign, sign_c(result.first.value()));
    }

    void Cpu::rotate_register_carry(const int reg, const int places, bool right)
    {
        auto value { registers.get(reg) };
        auto carry { flag_register.get_flag(flags::carry )};

        auto result { right ? rcr(value, carry, low_trits(places, 4))
            : rcl(value, carry, low_trits(places, 4)) };

        registers.set(reg, result.first);

        flag_register.set_flag(flags::carry, result.second);
        flag_register.set_flag(flags::sign, sign_c(result.first.value()));
    }

    void Cpu::set_register(const int reg, const int value)
    {
        registers.set(reg, value * (pow3(Word::word_size)/2));
    }

    void Cpu::move_register(const int srcreg, const int destreg)
    {
        registers.set(destreg, registers.get(srcreg));
    }

    void Cpu::exchange_registers(const int lreg, const int rreg)
    {
        auto temp { registers.get(lreg) };
        registers.set(lreg, registers.get(rreg));
        registers.set(rreg, temp);
    }

    void Cpu::set_flag_to_value(flags f, const int target)
    {
        flag_register.set_flag(f, target);
    }

    void Cpu::branch_absolute(const int address)
    {
        instruction_pointer.set(address);
    }

    void Cpu::branch_register(const int reg, const int disp, bool subroutine)
    {
        if (subroutine)
        {
            // Save the previous address on the stack

            // rs = stack pointer
            auto sp { registers.get(-6) };

            memory.set_word(sp.value(), instruction_pointer);

            auto newsp { sub(sp, 3) };

            // TODO: Handle stack overflow, probably using carry from the
            // previous operation.
            registers.set(-6, newsp.first);
        }

        auto newaddr { add(registers.get(reg), disp).first };
        instruction_pointer.set(newaddr);
    }

    void Cpu::branch_relative(const int disp)
    {
        auto newaddr { add(instruction_pointer, disp).first };
        instruction_pointer.set(newaddr);
    }

    void Cpu::branch_ternary(flags f, const int preg, const int zreg, const int nreg)
    {
        auto addr { instruction_pointer };

        switch (flag_register.get_flag(f))
        {
            case 1:
                addr.set(add(addr, preg).first);
                break;
            case 0:
                addr.set(add(addr, zreg).first);
                break;
            case -1:
                addr.set(add(addr, nreg).first);
                break;
            default:
                assert(0);
                break;
        }

        instruction_pointer.set(addr);
    }

    void Cpu::branch_call(const int addr, bool relative)
    {
        auto newaddr { relative ? add(instruction_pointer, addr).first : addr };

        // Save the previous address on the stack

        // rs = stack pointer
        auto sp { registers.get(-6) };

        memory.set_word(sp.value(), instruction_pointer);

        auto newsp  { sub(sp, 3) };

        // TODO: Handle stack overflow, probably using carry from the
        // previous operation.
        registers.set(-6, newsp.first);

        instruction_pointer.set(newaddr);
    }

    void Cpu::branch_return()
    {
        // Pop the stack to get the return address
        auto sp { registers.get(-6) };
        auto newsp { add(sp, 3) };

        // TODO: Handle stack overflow, probably using carry from the
        // previous operation.
        registers.set(-6, newsp.first);

        auto retaddr { memory.get_word(newsp.first.value()) };
        instruction_pointer.set(retaddr);
    }

    void Cpu::system_call(const int vec)
    {
        // rv = system call vector table
        auto vbase { registers.get(-9) };
        auto addr { add(vbase, vec*3).first };

        // rr = return address (system calls are not reentrant)
        registers.set(-5, instruction_pointer);

        instruction_pointer.set(addr);
    }

    void Cpu::system_return()
    {
        instruction_pointer.set(registers.get(-5));
    }

    void Cpu::system_breakpoint()
    {
        // if (flag_register.get_flag(flags::trap))
        // {
            throw debug_breakpoint{};
        // }
    }

    void Cpu::system_load_register(const int userreg, const int sysreg)
    {
        // The instruction pointer and flags register are aliased as CR0 and CR1,
        // so we have to handle them with a bit of special casing.

        if (sysreg > 0)
        {
            if (sysreg == 1)
            {
                registers.set(userreg, flag_register.get());
            }
            else
            {
                registers.set(userreg, control_regs.at(sysreg));
            }            
        }
        else if (sysreg < 0)
        {
            registers.set(userreg, debug_regs.at(sysreg));
        }
        else
        {
            registers.set(userreg, instruction_pointer);
        }
    }

    void Cpu::system_store_register(const int userreg, const int sysreg)
    {
        // The instruction pointer and flags register are aliased as CR0 and CR1,
        // so we have to handle them with a bit of special casing.

        if (sysreg < 0)
        {
            debug_regs.at(sysreg) = registers.get(userreg);
        }
        else if (flag_register.get_flag(flags::protection))
        {
            if (sysreg > 0)
            {
                if (sysreg == 1)
                {
                    const auto valid_flags = {0, 1, 2, 5, 6, 7, 8};

                    for (auto f : valid_flags)
                    {
                        flag_register.set_flag(flags(f), nth_trit(registers.get(userreg).value(), f));
                    }
                }
                else
                {
                    control_regs.at(sysreg) = registers.get(userreg);
                }                
            }
            else if (flag_register.get_flag(flags::protection) == -1)
            {
                // P flag = -1 allows directly changing IP
                instruction_pointer.set(registers.get(userreg));
            }
            else
            {
                throw protection_violation{};
            }
        }
        else
        {
            throw protection_violation{};
        }
    }

    void Cpu::io_read(const int reg, const int port, bool binary)
    {
        Word data { binary ? io.read_binary(port) : io.read(port) };

        if (binary)
        {
            registers.set(reg, bin(data));
        }
        else
        {
            registers.set(reg, data);
        }

        flag_register.set_flag(flags::sign, sign_c(data.value()));
    }

    void Cpu::io_write(const int reg, const int port, bool binary)
    {
        Word data { binary ? tri(registers.get(reg)) : registers.get(reg) };

        if (binary)
        {
            io.write_binary(port, static_cast<unsigned char>(data.value()));
        }
        else
        {
            io.write(port, data);
        }
    }
}
