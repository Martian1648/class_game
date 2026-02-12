/*
ὄνομα: player
αἰών: 1/19/2026
βούλημα: game
*/

#include "gameobject.h"
#include "physics.h"


GameObject::GameObject(const Vec<float> &position, const Vec<int> &size, World& world)
    :size{size}{
    physics.position = position;
}

GameObject::~GameObject() {

}



void GameObject::input(World& world) {
    const bool *key_states = SDL_GetKeyboardState(NULL);
    physics.acceleration.x = 0;
    // if (key_states[SDL_SCANCODE_W]) {
    //     velocity.y += -16;
    // }
    // if (key_states[SDL_SCANCODE_S]) {
    //     velocity.y += 16;
    // }
    if (key_states[SDL_SCANCODE_A]) {
        //velocity.x += -16;
        physics.acceleration.x += -physics.walk_acceleration;
    }
    if (key_states[SDL_SCANCODE_D]) {
        //velocity.x += 16;
        physics.acceleration.x += physics.walk_acceleration;
    }
    if (key_states[SDL_SCANCODE_SPACE]) {
        physics.velocity.y = physics.jump_velocity;
    }
}

void GameObject::update(World& world, double dt) {

}


std::pair<Vec<float>, Color> GameObject::get_sprite() const {
    return {physics.position, {255,0,2,100}};
}


