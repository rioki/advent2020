
#include "Assembler.h"

#include <cassert>
#include <string>

#include "../utils/strex.h"

using namespace strex;

namespace adv
{
    OpCode str2op(const std::string& str)
    {
        switch (hash(str))
        {
            case hash("nop"):
                return OpCode::NOP;
            case hash("jmp"):
                return OpCode::JMP;
            case hash("acc"):
                return OpCode::ACC;
            default:
                assert(false);
                return OpCode::NOP;
        }
    }

    Code Assembler::assemble(std::istream& input)
    {
        Code code;
        while (input.good())
        {
            std::string line;
            std::getline(input, line);

            auto tokens = tokenize(line, {" ", "+", "-"});
            assert(tokens.size() == 4);

            auto op = str2op(tokens[0]);
            auto arg = stoi(tokens[3]);
            if (tokens[2] == "-")
            {
                arg = -arg;
            }
            code.append(op, arg);
        }

        return code;
    }
}
