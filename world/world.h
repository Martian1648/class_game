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
#include "level.h"
#include "tilemap.h"

class Event;

class GameObject;
class Audio;
class World {
public:
    World(const Level& level, Audio& audio, GameObject* player,std::map<std::string, Event*>& events);

    void add_platform(float x, float y, float width, float height);
    bool collides(const Vec<float>&position) const;
    void update(float dt);
    void move_to(Vec<float>& position, const Vec<int>& size, Vec<float>& velocity);
    void load_level(const Level& level);
    bool end_level{false};
    Tilemap tilemap;
    void touch_tiles(GameObject& obj);
    Audio* audio;
    GameObject* player;
    std::map<std::string, Event*> events;
    std::vector<GameObject>game_objects;
};
