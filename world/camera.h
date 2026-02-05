/*
ὄνομα: camera
αἰών: 2/4/2026
βούλημα: game
*/

#pragma once
#include <vec.h>
#include "tilemap.h"


class Graphics;
class Color;
class Camera {
public:
    Camera(Graphics& graphics, float tilesize);

    void update(const Vec<float>& new_location, float dt);
    Vec<float> world_to_screen(const Vec<float>& world_position) const;

    void render(const Vec<float>& position, const Color& color, bool filled=true);
    void render(const Tilemap& tilemap, bool grid_on=false) const;

private:
    Graphics &graphics;
    float tilesize;
    Vec<float> location;

    void calculate_visible_tiles();
    Vec<int> visible_min, visible_max;
    Vec<float> goal, velocity, acceleration;
};