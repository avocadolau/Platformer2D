#pragma once

#include "SDL/include/SDL.h"

struct Clock
{
    uint64_t last_tick_time = 0;
    uint64_t delta = 0;

    void tick()
    {
        uint64_t tick_time = SDL_GetTicks();
        delta = tick_time - last_tick_time;
        last_tick_time = tick_time;
    }
};