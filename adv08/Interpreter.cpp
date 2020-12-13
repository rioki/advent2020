
#include "Interpreter.h"

#include <cassert>

namespace adv
{
    int32_t Interpreter::get_instruciton_pointer() const
    {
        return ip;
    }

    int32_t Interpreter::get_accumulator() const
    {
        return acc;
    }

    void Interpreter::run(const Code& code)
    {
        running = true;
        ip = 0;
        acc = 0;
        while (running && ip < code.size())
        {
            auto [op, arg] = code.get(ip);

            if (instruction_cb)
            {
                instruction_cb();
                if (!running)
                {
                    break;
                }
            }

            switch (op)
            {
                case OpCode::NOP:
                    ip++;
                    break;
                case OpCode::JMP:
                    ip += arg;
                    break;
                case OpCode::ACC:
                    acc += arg;
                    ip++;
                    break;
                default:
                    assert(false);
                    break;
            }
        }
    }

    void Interpreter::stop()
    {
        running = false;
    }

    void Interpreter::before_instruction(const std::function<void ()>& cb)
    {
        instruction_cb = cb;
    }
}
