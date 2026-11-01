//
//  sdl2.cpp
//  SDL2++
//
//  Created by Nevin Flanagan on 12/4/22.
//

#include "sdl2.h"

#include <new>

sdl2::lib::lib(unsigned int flags)
{
    SDL_InitSubSystem(flags);
}
sdl2::lib& sdl2::lib::operator |=(unsigned int flags)
{
    SDL_InitSubSystem(flags);
    return *this;
}
sdl2::lib::~lib()
{
    SDL_Quit();
}

template<>
sdl2::duration<unsigned long> sdl2::lib::time<unsigned long>() const
{
    return sdl2::duration<unsigned long>{ SDL_GetTicks() };
}

template<>
sdl2::seconds sdl2::lib::time<float>() const
{
    return seconds{ SDL_GetTicks() / 1000.0f };
}

template<>
sdl2::ticks sdl2::lib::time<unsigned long long>() const
{
    return sdl2::ticks{ SDL_GetTicks64() };
}

template<>
sdl2::duration<double> sdl2::lib::time<double>() const
{
    return sdl2::duration<double> { SDL_GetTicks64() / 1000.0 };
}

const sdl2::rect::filled& sdl2::rect::fill() const { return *new (*this) filled; }
const sdl2::rect::framed& sdl2::rect::draw() const { return *new (*this) framed; }
