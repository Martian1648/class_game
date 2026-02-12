/*
ὄνομα: world
αἰών: 1/19/2026
βούλημα: game
*/

#include "world.h"
#include <SDL3/SDL_rect.h>
#include <algorithm>

#include "physics.h"
#include "gameobject.h"

World::World(int width, int height):
tilemap{width,height}{

}


void World::add_platform(float x, float y, float width, float height) {
    SDL_FRect rect {x,y,width,height};
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tilemap(x+j, y+i) = Tile::Platform;
        }
    }
}



bool World::collides(const Vec<float> &position) const {
    int x = std::floor(position.x);
    int y = std::floor(position.y);

    return tilemap(x,y) == Tile::Platform;
}


GameObject *World::create_player() {
    player = std::make_unique<GameObject>(Vec<float>{10,1}, Vec<float>{64,64},*this);
    return player.get();
}

void World::move_to(Vec<float> &position, const Vec<float> &size, Vec<float> &velocity) {
    int x = std::floor(position.x);
    int u = x+tilemap.width;
    int y = std::floor(position.y);
    int v = y + tilemap.height;
    bool bottom, top, left, right;
    bottom = top = left = right = false;
    //bottom
    if (tilemap(x, std::max(v,tilemap.height)) == Tile::Platform) {
        position.y = std::ceil(y);
        velocity.y = 0;
        bottom = true;
    }
    //top
    if (tilemap(x,std::min(y,0)) == Tile::Platform) {
        position.y = std::ceil(y);
        velocity.y = 0;
        top = true;
    }
    //left
    if (tilemap(std::min(v,0), y) == Tile::Platform) {
        position.x = std::ceil(x);
        velocity.x = 0;
        left = true;
    }
    //right
    if (tilemap(std::max(tilemap.width,x), y) == Tile::Platform) {
        position.x = std::ceil(x);
        velocity.x = 0;
        right = true;
    }
}


void World::update(float dt) {
    // currently only players
    auto position = player->physics.position;
    auto velocity = player->physics.velocity;
    auto acceleration = player->physics.acceleration;

    velocity += 0.5f * acceleration * dt;
    position += velocity * dt;
    velocity += 0.5f * acceleration * dt;
    velocity.x *= player->physics.damping;

    velocity.x = std::clamp(velocity.x, -player->physics.terminal_velocity, player->physics.terminal_velocity);
    velocity.y = std::clamp(velocity.y, -player->physics.terminal_velocity, player->physics.terminal_velocity);

    //xheck for x

    Vec<float> future{position.x, player->physics.position.y};
    Vec<float> future_vel{velocity.x, 0};
    move_to(future, player->size, future_vel);
    //check for collisions
    /*if (collides(future)) {
        player->physics.velocity.x = 0;
        player->physics.acceleration.x = 0;
    }
    else {
        player->physics.velocity.x = velocity.x;
        player->physics.position.x = position.x;
        player->physics.acceleration.x = acceleration.x;
    }*/



    future.y = position.y;
    future_vel.y = velocity.y;
    move_to(future, player->size, future_vel);
    /*if (collides(future)) {
        player->physics.velocity.y = 0;
        player->physics.acceleration.y = 0;
    }
    else {
        player->physics.velocity.y = velocity.y;
        player->physics.position.y = position.y;
        player->physics.acceleration.y = player->physics.gravity;
    }*/

}

