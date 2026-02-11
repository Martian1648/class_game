/*
ὄνομα: toggle
αἰών: 2/9/2026
βούλημα: game
*/

#pragma once
#include <../cmake-build-debug-mingw/_deps/sdl-src/include/SDL3/SDL_stdinc.h>

class Toggle {
public:
    Toggle(bool on=false, double required_time = .25);
    void flip();
    bool on;
private:
    double required_time;
    double elapsed;
    Uint64 performance_frequency;
    Uint64 prev_counter;
};
