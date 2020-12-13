
#include <iostream>
#include <fstream>

#include "Code.h"
#include "Assembler.h"
#include "Interpreter.h"

int main()
{
    std::ifstream input("input.txt");

    adv::Assembler assembler;
    auto code = assembler.assemble(input);

    adv::Interpreter intepreter;

    std::vector<unsigned int> visited(code.size(), 0);
    intepreter.before_instruction([&] () {
        auto ip = intepreter.get_instruciton_pointer();
        visited[ip]++;
        if (visited[ip] > 1)
        {
            intepreter.stop();
        }
    });

    intepreter.run(code);

    std::cout << "Acc is " << intepreter.get_accumulator() << std::endl;

    // bruteforce swap jmp in nop
    for (auto i = 0u; i < code.size(); i++)
    {
        auto [op, arg] = code.get(i);
        if (op == adv::OpCode::JMP)
        {
            visited = std::vector<unsigned int>(code.size(), 0);
            auto copy = code;
            copy.update(i, adv::OpCode::NOP, arg);

            intepreter.run(copy);

            if (code.size() == intepreter.get_instruciton_pointer())
            {
                std::cout << "jmp -> nop inst " << i << " acc = " << intepreter.get_accumulator() << std::endl;
            }
        }
    }

    // bruteforce swap nop in jmp
    for (auto i = 0u; i < code.size(); i++)
    {
        auto [op, arg] = code.get(i);
        if (op == adv::OpCode::NOP)
        {
            visited = std::vector<unsigned int>(code.size(), 0);
            auto copy = code;
            copy.update(i, adv::OpCode::JMP, arg);

            intepreter.run(copy);

            if (code.size() == intepreter.get_instruciton_pointer())
            {
                std::cout << "nop -> jmp inst " << i << " acc = " << intepreter.get_accumulator() << std::endl;
            }
        }
    }

    std::cout << "Done" << std::endl;
}

