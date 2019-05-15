#include "cpu.hpp"

namespace ternary
{
    void Cpu::decode_major(Opcode& op)
    {
        switch (op.o)
        {
            case 0:
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
                undefined_opcode(op);
                break;
            case -7:
                undefined_opcode(op);
                break;
            case -8:
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
                compare_immediate(op.y, op.low6());
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

    void Cpu::undefined_opcode(Opcode& op)
    {
        // TODO: Trap and issue a simulated CPU interrupt
        std::cerr << "Undefined operation " << op.value.value_string() << '\n';
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
            Word w { memory.get_word(addr) };
            registers.set(reg, w);

            flag_register.set_flag(flags::sign, sign_c(w.value()));
        }
        else
        {
            Hexad h { memory.get(addr) };
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
            current.set(memory.get_word(address.value()));
        }
        else
        {
            auto data { memory.get(address.value()) };

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

        registers.set(destreg, memory.get_word(address));
        
        if (flag_register.get_flag(flags::direction))
        {
            registers.set(destreg, add(address, 3).first);
        }

        flag_register.set_flag(flags::sign, sign_c(registers.get(destreg).value()));
    }

    void Cpu::store_register_memory(const int reg, const int addr, hexad_select type)
    {
        Word r { registers.get(reg) };
        if (type == hexad_select::full_word)
        {
            memory.set_word(addr, r);
        }
        else
        {
            switch (type)
            {
                 case hexad_select::low:
                    memory.set(addr, r.low());
                    break;

                case hexad_select::middle:
                    memory.set(addr, r.middle());
                    break;
                
                case hexad_select::high:
                    memory.set(addr, r.high());
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
            memory.set_word(address.value(), current);
        }
        else
        {
            switch (type)
            {
                 case hexad_select::low:
                    memory.set(address.value(), current.low().get());
                    break;

                case hexad_select::middle:
                    memory.set(address.value(), current.middle().get());
                    break;
                
                case hexad_select::high:
                    memory.set(address.value(), current.high().get());
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

        memory.set_word(address, registers.get(srcreg));

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
            instruction_pointer = { addr };
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
            // Divide by zero
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
            // Divide by zero
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
}
