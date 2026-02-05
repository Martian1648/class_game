/*
ὄνομα: player
αἰών: 1/19/2026
βούλημα: game
*/

#include "player.h"
#include "physics.h"


Player::Player(const Vec<float> &position, const Vec<float> &size)
    :position{position}, size{size}, velocity{0,0}{
    acceleration.y = gravity;
}




void Player::handle_input() {
    const bool *key_states = SDL_GetKeyboardState(NULL);
    acceleration.x = 0;
    // if (key_states[SDL_SCANCODE_W]) {
    //     velocity.y += -16;
    // }
    // if (key_states[SDL_SCANCODE_S]) {
    //     velocity.y += 16;
    // }
    if (key_states[SDL_SCANCODE_A]) {
        //velocity.x += -16;
        acceleration.x += -walk_acceleration;
    }
    if (key_states[SDL_SCANCODE_D]) {
        //velocity.x += 16;
        acceleration.x += walk_acceleration;
    }
    if (key_states[SDL_SCANCODE_SPACE]) {
        velocity.y = -jump_velocity;
    }
}



std::pair<SDL_FRect, Color> Player::get_sprite() const {
    return {{position.x, position.y, size.x, size.y}, {255,0,2,100}};
}


