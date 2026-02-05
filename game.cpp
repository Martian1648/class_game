/*
ὄνομα: game
αἰών: 1/28/2026
βούλημα: game
*/

#include "game.h"

Game::Game(std::string title, int width, int height) :
graphics{title, width, height},world{32,11}, dt{1.0/60.0}, lag{0.0},
performance_frequency(SDL_GetPerformanceFrequency()),
prev_counter(SDL_GetPerformanceCounter()){
    //load the first level
    //boundaries
    world.add_platform(0,0,30,1);
    world.add_platform(0,0,1,10);
    world.add_platform(30,0,1,10);
    world.add_platform(0,10,31,1);

    //platforms
    world.add_platform(3,7,4,1);
    world.add_platform(13,4,6,1);

    player = world.create_player();
}

void Game::input() {
    player->handle_input();
}

void Game::update() {
    Uint64 now = SDL_GetPerformanceCounter();
    lag += (now - prev_counter) / (double) performance_frequency;
    prev_counter = now;
    while (lag >= dt) {
        world.update(dt);
        lag -= dt;
    }
}

void Game::render() {
    // clear
    graphics.clear();

    //draw the world
    for (auto &platform : world.get_platforms()) {
        graphics.draw(platform, {0,255,0,255});
    }
    auto [player_sprite, color] = player->get_sprite();
    graphics.draw(player_sprite, color);

    graphics.update();
}


