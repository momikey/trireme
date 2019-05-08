#include "cpu.hpp"

namespace ternary
{
    void Cpu::decode_major(Opcode& op)
    {
        switch (op.o)
        {
            case 0:
            case 1:
            case 2:
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
                break;
        }
    }

    void Cpu::decode_minor_arithmetic(Opcode& op)
    {
        
    }

    void Cpu::undefined_opcode(Opcode& op)
    {
        // TODO: Trap and issue a simulated CPU interrupt
        std::cerr << "Undefined operation " << op.value.value_string() << '\n';
    }

    void Cpu::load_register_memory(const int reg, const int addr, hexad_select type)
    {
        if (type == hexad_select::full_word)
        {
            Word w { memory.get(addr+2), memory.get(addr+1), memory.get(addr) };
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

    void Cpu::store_register_memory(const int reg, const int addr, hexad_select type)
    {
        Word r { registers.get(reg) };
        if (type == hexad_select::full_word)
        {
            memory.set(addr, r.low());
            memory.set(addr+1, r.middle());
            memory.set(addr+2, r.high());
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

    void Cpu::branch_on_flag(const int addr, const int flag, const int target)
    {
        if (flag_register.get_flag(static_cast<flags>(flag)) == target)
        {
            instruction_pointer = { addr };
        }
    }
}
