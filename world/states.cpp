/*
ὄνομα: states
αἰών: 2/16/2026
βούλημα: game
*/

#include "states.h"
#include "action.h"

void Standing::on_enter(World &, GameObject &obj) {
    obj.color = Color{0,255,255,255};
}

Action *Standing::input(World &world, GameObject &obj, ActionType action_type) {
    if (action_type == ActionType::Jump) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
    return nullptr;
}

void InAir::on_enter(World &, GameObject &) {
    elapsed = cooldown;
}

void InAir::update(World &world, GameObject &obj, double dt) {
    elapsed -= dt;
    if (elapsed <= 0) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}


