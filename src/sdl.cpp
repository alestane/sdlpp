//
//  sdl2.cpp
//  SDL2++
//
//  Created by Nevin Flanagan on 12/4/22.
//

#include "./_def"
#include INTERNAL(sdl.h)

#include <new>

namespace _sdl {
    lib::lib(unsigned int flags)
    {
        SDL_InitSubSystem(flags);
    }
    lib& lib::operator |=(unsigned int flags)
    {
        SDL_InitSubSystem(flags);
        return *this;
    }
    lib::~lib()
    {
        SDL_Quit();
    }

    template<>
    duration<unsigned long> lib::time<unsigned long>() const
    {
        return duration<unsigned long>{ SDL_GetTicks() };
    }

    template<>
    seconds lib::time<float>() const
    {
        return seconds{ SDL_GetTicks() / 1000.0f };
    }

    template<>
    ticks lib::time<unsigned long long>() const
    {
        return ticks{ SDL_GetTicks64() };
    }

    template<>
    duration<double> lib::time<double>() const
    {
        return duration<double> { SDL_GetTicks64() / 1000.0 };
    }

    const rect::filled& rect::fill() const { return *new (*this) filled; }
    const rect::framed& rect::draw() const { return *new (*this) framed; }
}
