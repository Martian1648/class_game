/*
ὄνομα: states
αἰών: 2/16/2026
βούλημα: game
*/

#include "states.h"
#include "action.h"
#include "randoms.h"
#include "world.h"


//helper function
bool on_platform(World& world, GameObject& obj) {
    constexpr float epsilon = 1e-4;
    Vec<float> left_foot{obj.physics.position.x +  epsilon, obj.physics.position.y - epsilon};
    Vec<float> right_foot{obj.physics.position.x + obj.size.x-epsilon,obj.physics.position.y-epsilon};
    return world.collides(left_foot) || world.collides(right_foot);
}
void Standing::on_enter(World &, GameObject &obj) {
    obj.color = Color{0,255,255,255};
    obj.set_sprite("idle");
    obj.physics.acceleration.x = 0;
}

Action *Standing::input(World &world, GameObject &obj, ActionType action_type) {
    if (action_type == ActionType::Jump) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
    else if (action_type==ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight();
    }
    else if (action_type==ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft();
    }
    return nullptr;
}

void InAir::on_enter(World &world, GameObject &obj) {
    elapsed = cooldown;
    obj.set_sprite("jumping");
    obj.color = {255,0,0,255};
}

void InAir::update(World &world, GameObject &obj, double dt) {
    elapsed -= dt;
    if (elapsed <= 0 && on_platform(world,obj)) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}

void Running::on_enter(World &, GameObject &obj) {
    obj.color = {255,255,0, 255};
    obj.physics.acceleration.x /= obj.physics.sprint_acceleration_multiplier;
    obj.set_sprite("walking");
}

Action *Running::input(World &world, GameObject &obj, ActionType action_type) {
     if (action_type == ActionType::None) {
         obj.fsm->transition(Transition::Stop, world, obj);
     }
     if (action_type == ActionType::Jump) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
     if (action_type == ActionType::SprintLeft) {
        obj.fsm->transition(Transition::Plant, world, obj);
        return new SprintLeft();
    }
     if (action_type == ActionType::SprintRight) {
        obj.fsm->transition(Transition::Plant, world, obj);
        return new SprintRight();
    }
    if (action_type == ActionType::AttackAll) {
        obj.fsm->transition(Transition::AttackOmnes, world, obj);
        }
    return nullptr;
}


void Sprinting::on_enter(World &, GameObject &obj) {
    elapsed= cooldown;
    obj.color={0,255,0,255};
}

void Sprinting::update(World &world, GameObject &obj, double dt) {
    elapsed -= dt;

    if (elapsed <= 0) {
        obj.fsm->transition(Transition::Chill, world, obj);
    }
}

void AttackAllEnemies::on_enter(World &world, GameObject &obj) {
    obj.color = {255,100,0,255};
    for (auto& enemy: world.game_objects) {
        if (enemy == world.player) continue;
        enemy->take_damage(obj.damage);
    }
    elapsed = 0;
}

void AttackAllEnemies::update(World &world, GameObject &obj, double dt) {
    elapsed -= dt;

    if (elapsed <= 0) {
        obj.fsm->transition(Transition::Chill, world, obj);
    }
}

void Patroling::on_enter(World &world, GameObject &obj) {
    // set cooldown to a random amount of time
    elapsed = 0;
    cooldown = randint(3,10);
    Running::on_enter(world, obj);
}

Action *Patroling::input(World &world, GameObject &obj, ActionType action_type) {
    if (elapsed >= cooldown) {
        return Running::input(world, obj, ActionType::None);
    }
    return Running::input(world, obj, action_type);
}

void Patroling::update(World &, GameObject &, double dt) {
    elapsed+= dt;
}



