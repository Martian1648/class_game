/*
ὄνομα: action
αἰών: 2/13/2026
βούλημα: game
*/

#pragma once
#include "gameobject.h"

enum class ActionType {
    None,
    Jump,
    MoveLeft,
    MoveRight
};
class Action {
public:
    virtual ~Action() = default;
    virtual void perform(World& world, GameObject obj) = 0;
};

class Jump : public Action {
    void perform(World &, GameObject obj) override;
};