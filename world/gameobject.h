#pragma once
#include <SDL3/SDL.h>
#include "../graphics/graphics.h"
#include "utility"
#include "../utility/vec.h"

class World;

class GameObject {
public:
    GameObject(const Vec<float>& position, const Vec<float> &size, World& world);
    ~GameObject();
    void input(World& world);
    void update(World& world, double dt);
    std::pair<Vec<float>, Color> get_sprite() const;


    Vec<float> position;
    Vec<float> size;
    Vec<float> velocity;
    Vec<float> acceleration;


};