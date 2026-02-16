/*
ὄνομα: action
αἰών: 2/13/2026
βούλημα: game
*/

#include "action.h"

void Jump::perform(World &, GameObject obj) {
    obj.physics.velocity.y = obj.physics.jump_velocity;
}
