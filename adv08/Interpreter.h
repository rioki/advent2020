
#pragma once

#include <cstdint>
#include <functional>

#include "Code.h"

namespace adv
{
    class Interpreter
    {
    public:

        int32_t get_instruciton_pointer() const;

        int32_t get_accumulator() const;

        void run(const Code& code);

        void stop();

        void before_instruction(const std::function<void ()>& cb);

    private:
        bool running = false;
        int32_t ip = 0;
        int32_t acc = 0;
        std::function<void ()> instruction_cb;
    };
}
