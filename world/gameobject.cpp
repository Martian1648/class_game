/*
ὄνομα: player
αἰών: 1/19/2026
βούλημα: game
*/

#include "gameobject.h"

#include "action.h"
#include "physics.h"


GameObject::GameObject(const Vec<float> &position, const Vec<int> &size, World& world,
    FSM* fsm, Color color)
    :size{size},fsm{fsm}, color{color}{
    physics.position = position;
}

GameObject::~GameObject() {

}



void GameObject::input(World& world) {
const bool *key_states = SDL_GetKeyboardState(NULL);
    ActionType action_type;
/*physics.acceleration.x = 0;
physics.acceleration.y = physics.gravity;
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
*/
    if (key_states[SDL_SCANCODE_SPACE]) {
        //physics.velocity.y = physics.jump_velocity;
        action_type = ActionType::Jump;
    }
    Action* action = fsm->current_state->input(world, *this, action_type);
    if (action != nullptr) {
        action->perform(world, *this);
        delete action;
    }


}

void GameObject::update(World& world, double dt) {
    fsm->current_state->update(world, *this, dt);
}


std::pair<Vec<float>, Color> GameObject::get_sprite() const {
    return {physics.position, color};
}


