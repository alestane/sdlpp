//
//  sdl2.hpp
//  SDL2++
//
//  Created by Nevin Flanagan on 12/4/22.
//

#ifndef sdl2_h
#define sdl2_h

#include <SDL.h>

#include <memory>
#include <chrono>
#include <numeric>
#include <concepts>

namespace sdl2 {

    template <typename rep> requires std::unsigned_integral<rep> || std::floating_point<rep>
    using period = std::conditional_t<std::is_integral_v<rep>, std::chrono::milliseconds::period, std::chrono::seconds::period>;

    template <typename rep>
    using duration = std::chrono::duration<rep, period<rep>>;

    using ticks = duration<unsigned long long>;
    using seconds = duration<float>;

    class lib {

    public:
        enum init : unsigned int {
            timer = SDL_INIT_TIMER,
            audio = SDL_INIT_AUDIO,
            video = SDL_INIT_VIDEO,
            joystick = SDL_INIT_JOYSTICK,
            haptic = SDL_INIT_HAPTIC,
            game_controller = SDL_INIT_GAMECONTROLLER,
            events = SDL_INIT_EVENTS,
            everything = SDL_INIT_EVERYTHING,
        };
        lib(unsigned int flags = init::everything);
        lib& operator |=(unsigned int);
        ~lib();

        template<typename rep = ticks::rep, typename period = period<rep> >
        std::chrono::duration<rep, period> time() const;
    };

    template <typename T>
    struct point2;

    template <>
    struct point2<int> : public SDL_Point {
        using coord = int;
        constexpr point2() : point2{ 0, 0 } {}
        constexpr point2(coord x, coord y) : SDL_Point{ x, y } {}
    };

    template <>
    struct point2<float> : public SDL_FPoint {
        using coord = float;
        constexpr point2() noexcept : point2{ 0.0f, 0.0f } {}
        constexpr point2(coord x, coord y) noexcept : SDL_FPoint{ x, y } {}
        constexpr point2(const point2<int>& p) noexcept : point2{ static_cast<float>(x), static_cast<float>(y) } {}
        constexpr operator point2<int>() const noexcept {return { static_cast<int>(x), static_cast<int>(y) }; }
    };

    using point = point2<int>;
    using fpoint = point2<float>;
    using color = SDL_Color;
    using colour = SDL_Colour;

    template <typename T>
    inline point2<T> operator -(const point2<T>& p) { return { -p.x, -p.y }; }
    // magnitude operator
    template <typename T>
    inline int operator +(const point2<T>& p) { return hypot(p.x, p.y); } // TODO: optimize
    // normalized operator
    inline fpoint operator *(const fpoint& fp) { auto magnitude = +fp; return fpoint{ fp.x / magnitude, fp.y / magnitude }; }
    inline fpoint operator *(const point& p) { return *static_cast<fpoint>( p ); }

    inline color operator~(color o) {
        union { color c; uint32_t bits; } body{ o };
        body.bits = ~body.bits;
        return body.c;
    }

    template <typename T>
    struct rectangle;
    
    template <>
    struct rectangle<int> : public SDL_Rect {
        constexpr rectangle() = default;
        constexpr rectangle(int x, int y, int w, int h) : SDL_Rect{ x, y, w, h } {}

        operator bool() const { return !SDL_RectEmpty(this); }
        bool contains(const point& p) const { return SDL_PointInRect(&p, this); }
    };

    template <>
    struct rectangle<float> : public SDL_FRect {
        constexpr rectangle() noexcept = default;
        constexpr rectangle(float x, float y, float w, float h) : SDL_FRect{ x, y, w, h } {}
        constexpr rectangle(const rectangle<int>& r) : rectangle{ static_cast<float>(r.x), static_cast<float>(r.y), static_cast<float>(r.w), static_cast<float>(r.h) } {}
        operator rectangle<int>() const { return rectangle<int>{ static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h) }; }
        operator bool() const { return !SDL_FRectEmpty(this); }
        bool contains(const fpoint& p) const { return SDL_PointInFRect(&p, this); }
    };

    using frect = rectangle<float>;

    struct rect : public rectangle<int> {
        using rectangle<int>::rectangle;

        struct filled;
        struct framed;
        const filled& fill() const;
        const framed& draw() const;
    protected:
        void* operator new(size_t, const rect& r) { return &const_cast<rect&>(r); }
    };
    
    struct rect::filled : public rect { using rect::rect; };
    struct rect::framed : public rect { using rect::rect; };

    template<typename T>
    class frame : public rect {
    public:
        frame(T& host, int x, int y, int w = 1, int h = 1) : rect{ x, y, w, h }, _host{ host } {}
        frame(T& host, const rect& r) : rect{ r }, _host{ host } {}
        frame(T& host) : _host{ host } {}

        T& operator *() { return _host; }
        const T& operator *() const { return _host; }

    private:
        T& _host;
    };

    template <typename T>
    inline bool operator !(const rectangle<T>& r) noexcept { return !static_cast<bool>(r); }

    template <typename T>
    inline bool operator == (const point2<T>& a, const point2<T>& b) noexcept {}
    template <typename T>
    inline point2<T> operator + (const point2<T>& a, const point2<T>& b) { return { a.x + b.x, a.y + b.y }; }
    template <typename T>
    inline point2<T>& operator += (point2<T>& a, const point2<T>& b) { a.x += b.x; a.y += b.y; return a; }
    template <typename T>
    inline point2<T> operator - (const point2<T>& a, const point2<T>& b) { return { a.x - b.x, a.y - b.y }; }
    template <typename T>
    inline point2<T>& operator -= (point2<T>& a, const point2<T>& b) { a.x -= b.x; a.y -= b.y; return a; }
    template <typename T>
    inline point2<T> operator * (const point2<T>& p, T s) { return { p.x * s, p.y * s }; }
    template <typename T>
    inline point2<T> operator * (T s, const point2<T>& p) { return { p.x * s, p.y * s }; }
    template <typename T>
    inline point& operator *= (point& a, T s) { a.x *= s; a.y *= s; return a; }
    template <typename T>
    inline point2<T> operator / (const point2<T>& p, T s) { return { p.x / s, p.y / s }; }
    template <typename T>
    inline point2<T> operator / (T s, const point2<T>& p) { return { p.x / s, p.y / s }; }
    template <typename T>
    inline point2<T>& operator /= (point2<T>& a, T s) { a.x /= s; a.y /= s; return a; }

    inline bool operator == (const rectangle<int>& a, const rectangle<int>& b) { return SDL_RectEquals(&a, &b); }
    inline bool operator == (const frect& a, const frect& b) { return SDL_FRectEquals(&a, &b); }
    template <typename T>
    inline rectangle<T> operator + (const rect& r, const point& offset) { return { r.x + offset.x, r.y + offset.y, r.w, r.h }; }
    template <typename T>
    inline rectangle<T> operator - (const rectangle<T>& r, const point2<T>& offset) { return r + -offset; }
    template <typename T>
    inline rectangle<T> operator * (const rectangle<T>& r, const point2<T>& scale) { return { r.x * scale.x, r.y * scale.y, r.w * scale.x, r.h * scale.y }; }
    template <typename T>
    inline rectangle<T> operator / (const rectangle<T>& r, const point2<T>& scale) { return { r.x / scale.x, r.y / scale.y, r.w / scale.x, r.h / scale.y }; }
    template <typename T>
    inline rectangle<T> operator ^ (const rectangle<T>& r, const point2<T>& corner) { return { corner.x, corner.y, r.w, r.h }; }
    template <typename T>
    inline bool operator >=(const rect& bounds, const point2<T>& test) { return bounds.contains(test); }
    inline rect operator & (const rect& a, const rect& b) { rect target; return SDL_IntersectRect(&a, &b, &target) ? target : rect{ 0, 0, 0, 0 }; }
    inline rect operator | (const rect& a, const rect& b) { rect target; SDL_UnionRect(&a, &b, &target); return target; }
    
    inline frect operator & (const frect& a, const frect& b) { frect target; return SDL_IntersectFRect(&a, &b, &target) ? target : frect{ 0, 0, 0, 0 }; }
    inline frect operator | (const frect& a, const frect& b) { frect target; SDL_UnionFRect(&a, &b, &target); return target; }

    template <>
    duration<unsigned long> lib::time<unsigned long>() const;
    template <>
    ticks lib::time<unsigned long long>() const;
    template <>
    seconds lib::time<float>() const;
    template <>
    duration<double> lib::time<double>() const;
}

#endif 
