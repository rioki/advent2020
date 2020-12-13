
#include "Code.h"

#include <cassert>

namespace adv
{
    std::tuple<OpCode, int32_t> Code::get(int32_t index) const
    {
        if (index < code.size())
        {
            return code[index];
        }
        else
        {
            return std::make_tuple(OpCode::NOP, 0);
        }
    }

    size_t Code::size() const
    {
        return code.size();
    }

    void Code::update(int32_t index, OpCode op, int32_t arg)
    {
        assert(index < code.size());
        code[index] = std::make_tuple(op, arg);
    }

    void Code::append(OpCode op, int32_t arg)
    {
        code.push_back(std::make_tuple(op, arg));
    }
}
