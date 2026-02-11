/*
ὄνομα: world
αἰών: 1/19/2026
βούλημα: game
*/

#include "world.h"
#include <SDL3/SDL_rect.h>
#include <algorithm>

#include "physics.h"
#include "player.h"

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


Player *World::create_player() {
    player = std::make_unique<Player>(Vec<float>{10,5}, Vec<float>{64,64});
    return player.get();
}

void World::update(float dt) {
    // currently only players
    auto position = player->position;
    auto velocity = player->velocity;
    auto acceleration = player->acceleration;

    velocity += 0.5f * acceleration * dt;
    position += velocity * dt;
    velocity += 0.5f * acceleration * dt;
    velocity.x *= damping;

    velocity.x = std::clamp(velocity.x, -terminal_velocity, terminal_velocity);
    velocity.y = std::clamp(velocity.y, -terminal_velocity, terminal_velocity);

    //xheck for x

    Vec<float> future{position.x, player->position.y};
    //check for collisions
    if (collides(future)) {
        player->velocity.x = 0;
        player->acceleration.x = 0;
    }
    else {
        player->velocity.x = velocity.x;
        player->position.x = position.x;
        player->acceleration.x = acceleration.x;
    }

    future.x = player->position.x;

    future.y = position.y;
    if (collides(future)) {
        player->velocity.y = 0;
        player->acceleration.y = 0;
    }
    else {
        player->velocity.y = velocity.y;
        player->position.y = position.y;
        player->acceleration.y = gravity;
    }

}

