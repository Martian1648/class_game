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
#include "states.h"

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
    if (x >= tilemap.width || x < 0 || y >= tilemap.height || y < 0) {
        return true;
    }
    return tilemap(x,y) == Tile::Platform;
}


GameObject *World::create_player() {
    Transitions transitions = {
        {{StateType::Standing, Transition::Jump}, StateType::InAir},
        {{StateType::InAir, Transition::Stop}, StateType::Standing}
    };
    States states = {
        {StateType::Standing, new Standing()},
        {StateType::InAir, new InAir()}
    };
    FSM* fsm = new FSM{transitions, states, StateType::Standing};
    player = std::make_unique<GameObject>(Vec<float>{10,1}, Vec<int>{1, 1},*this, fsm,
        Color{255,0,0,255});
    return player.get();
}

/*void World::move_to(Vec<float> &position, const Vec<int> &size, Vec<float> &velocity) {
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
}*/


void World::update(float dt) {
    // currently only updating player
    auto position = player->physics.position;
    auto velocity = player->physics.velocity;
    auto acceleration = player->physics.acceleration;

    velocity += 0.5f * acceleration * dt;
    position += velocity * dt;
    velocity += 0.5f * acceleration * dt;
    velocity.x *= player->physics.damping;

    velocity.x = std::clamp(velocity.x, -player->physics.terminal_velocity, player->physics.terminal_velocity);
    velocity.y = std::clamp(velocity.y, -player->physics.terminal_velocity, player->physics.terminal_velocity);

    // check for x collisions
    // Check for collisions with the world - x direction
    Vec<float> future_position{position.x, player->physics.position.y};
    Vec<float> future_velocity{velocity.x, 0};
    move_to(future_position, player->size, future_velocity);

    // y direction attempt after (maybe) moving in x
    future_velocity.y = velocity.y;
    future_position.y = position.y;
    move_to(future_position, player->size, future_velocity);

    // update player
    player->physics.position = future_position;
    player->physics.velocity = future_velocity;
}

void World::move_to(Vec<float>& position, const Vec<int>& size, Vec<float>& velocity) {
    // test sides first. if both collide move backward
    // bottom side
    if (collides(position) && collides({position.x + size.x, position.y})) {
        position.y = std::ceil(position.y);
        velocity.y = 0;
    }
    // top side
    else if (collides({position.x, position.y + size.y}) && collides({position.x + size.x, position.y + size.y})) {
        position.y = std::floor(position.y);
        velocity.y = 0;
    }
    // left side
    if (collides(position) && collides({position.x, position.y + size.y})) {
        position.x = std::ceil(position.x);
        velocity.x = 0;
    }
    // right side
    else if (collides({position.x + size.x, position.y}) && collides({position.x + size.x, position.y + size.y})) {
        position.x = std::floor(position.x);
        velocity.x = 0;
    }
    // test corners next, move back in smaller axis
    if (collides(position)) {
        float dx = std::ceil(position.x) - position.x;
        float dy = std::ceil(position.y) - position.y;
        if (dx > dy) {
            position.y = std::ceil(position.y);
            velocity.y = 0;
        }
        else {
            position.x = std::ceil(position.x);
            velocity.x = 0;
        }
    }
    else if (collides({position.x, position.y + size.y})) {
        float dx = std::ceil(position.x) - position.x;
        float dy = position.y - std::floor(position.y);
        if (dx > dy) {
            position.y = std::floor(position.y);
            velocity.y = 0;
        }
        else {
            position.x = std::ceil(position.x);
            velocity.x = 0;
        }
    }
    else if (collides({position.x + size.x, position.y})) {
        float dx = position.x - std::floor(position.x);
        float dy = std::ceil(position.y) - position.y;
        if (dx > dy) {
            position.y = std::ceil(position.y);
            velocity.y = 0;
        }
        else {
            position.x = std::floor(position.x);
            velocity.x = 0;
        }
    }
    else if (collides({position.x + size.x, position.y + size.y})) {
        float dx = position.x - std::floor(position.x);
        float dy = position.y - std::floor(position.y);
        if (dx > dy) {
            position.y = std::floor(position.y);
            velocity.y = 0;
        }
        else {
            position.x = std::floor(position.x);
            velocity.x = 0;
        }
    }
}

