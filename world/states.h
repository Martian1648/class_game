/*
ὄνομα: states
αἰών: 2/16/2026
βούλημα: game
*/

#pragma once
#include "fsm.h"

class Standing : public State {
public:
    virtual void on_enter(World &, GameObject &) override;
    virtual Action* input(World &, GameObject &, ActionType) override;
};

class InAir : public State {
public:
    virtual void on_enter(World &, GameObject &) override;
    virtual void update(World &, GameObject &, double dt) override;

    const double cooldown = 1;
    double elapsed = 0;
};