#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <cmath>
#include <SDL2/SDL.h>
#include "map.hpp"
#include "vec2.hpp"


const SDL_Color SKY_COLOR = {0, 0, 0, 255};
const SDL_Color WALL_COLOR = {0, 0, 255, 255};


class Camera
{
public:
    Camera(Map* map, const Vec2& position, const Vec2& direction,
           double fov, double nearDistance, double drawDistance);
    
    void moveX(double dx);      // move camera dx units to the right
    void moveZ(double dz);      // move camera dz units forward
    void rotate(double angle);  // rotate camera CCW
    
    void render(SDL_Surface* surface) const; // render scene to given surface
    
private:
    // Returns world-space coords of column of pixels on the screen
    Vec2 findColumn(int x, int w) const;
    
    const Map* map; // map that the camera can see
    
    Vec2 position;  // viewpoint from which scene is rendered
    
    // the camera's unit vectors in world-space
    Vec2 xUnit; // toward right side of screen
    Vec2 zUnit; // into screen
    
    double nearDistance;    // distance to near clip plane
    double drawDistance;    // distance to far clip plane
    double viewRadius;      // center of screen to right edge
};

#endif
