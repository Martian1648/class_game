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
    player = std::make_unique<GameObject>(Vec<float>{10,1}, Vec<int>{1, 1},*this);
    return player.get();
}

void World::move_to(Vec<float> &position, const Vec<int> &size, Vec<float> &velocity) {
    int left_tile = std::floor(position.x);
    int right_tile = std::floor(position.x + size.x);
    int top_tile = std::floor(position.y);
    int bottom_tile = std::floor(position.y + size.y);

    if (velocity.y > 0) {
        for (int x = left_tile; x <= right_tile; ++x) {
            if (x >= 0 && x < tilemap.width && bottom_tile >= 0 && bottom_tile < tilemap.height) {
                if (tilemap(x, bottom_tile) == Tile::Platform) {
                    position.y = std::floor(position.y + size.y) - ( size.y) ;

                    velocity.y = 0;
                    break;
                }
            }
        }
    }
    else if (velocity.y < 0) {
        for (int x = left_tile; x <= right_tile; ++x) {
            if (x >= 0 && x < tilemap.width && top_tile >= 0 && top_tile < tilemap.height) {
                if (tilemap(x, top_tile) == Tile::Platform) {
                    position.y = std::ceil(position.y);
                    velocity.y = 0;
                    break;
                }
            }
        }
    }
    top_tile = std::floor(position.y);
    bottom_tile = std::floor(position.y + size.y);
    if (velocity.x > 0) {
        for (int y = top_tile; y <= bottom_tile; ++y) {
            if (right_tile >= 0 && right_tile < tilemap.width && y >= 0 && y < tilemap.height) {
                if (tilemap(right_tile, y) == Tile::Platform) {
                    position.x = std::floor(position.x + size.x) - size.x;
                    velocity.x = 0;
                    break;
                }
            }
        }
    }
    else if (velocity.x < 0) {
        for (int y = top_tile; y <= bottom_tile; ++y) {
            if (left_tile >= 0 && left_tile < tilemap.width && y >= 0 && y < tilemap.height) {
                if (tilemap(left_tile, y) == Tile::Platform) {
                    position.x = std::ceil(position.x);
                    velocity.x = 0;
                    break;
                }
            }
        }
    }
}


void World::update(float dt) {
    // currently only players
    auto position = player->physics.position;
    auto velocity = player->physics.velocity;
    auto acceleration = player->physics.acceleration;
    acceleration.y = player->physics.gravity;

    velocity += 0.5f * acceleration * dt;
    position += velocity * dt;
    velocity += 0.5f * acceleration * dt;
    velocity.x *= player->physics.damping;

    velocity.x = std::clamp(velocity.x, -player->physics.terminal_velocity, player->physics.terminal_velocity);
    velocity.y = std::clamp(velocity.y, -player->physics.terminal_velocity, player->physics.terminal_velocity);

    //xheck for x

    Vec<float> future{position.x, player->physics.position.y};
    Vec<float> future_vel{velocity.x, 0};
    move_to(future, {1,1}, future_vel);
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

    player->physics.position.x = future.x;
    player->physics.velocity.x = future_vel.x;

    future.y = position.y;
    future_vel.y = velocity.y;
    future_vel.x = 0;
    move_to(future, {1,1}, future_vel);
    /*if (collides(future)) {
        player->physics.velocity.y = 0;
        player->physics.acceleration.y = 0;
    }
    else {
        player->physics.velocity.y = velocity.y;
        player->physics.position.y = position.y;
        player->physics.acceleration.y = player->physics.gravity;
    }*/
    player->physics.position.y = future.y;
    player->physics.velocity.y = future_vel.y;
}

