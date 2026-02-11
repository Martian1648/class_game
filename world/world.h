/*
ὄνομα: world
αἰών: 1/19/2026
βούλημα: game
*/

#pragma once
#include <memory>
#include <SDL3/SDL.h>
#include <vector>
#include <vec.h>

#include "tilemap.h"

class Player;

class World {
public:
    World(int width, int height);

    void add_platform(float x, float y, float width, float height);
    bool collides(const Vec<float>&position) const;
    Player* create_player();
    void update(float dt);

    Tilemap tilemap;
private:

    std::unique_ptr<Player> player;
};
