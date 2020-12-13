
#pragma once

#include <cstdint>
#include <vector>
#include <tuple>

namespace adv
{
    enum class OpCode
    {
        NOP,
        JMP,
        ACC
    };

    struct Code
    {
    public:

        std::tuple<OpCode, int32_t> get(int32_t index) const;

        size_t size() const;

        void update(int32_t index, OpCode op, int32_t arg);

        void append(OpCode op, int32_t arg);

    private:
        std::vector<std::tuple<OpCode, int32_t>> code;
    };
}
