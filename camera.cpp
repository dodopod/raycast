#include "camera.hpp"

Camera::Camera(Map* map, const Vec2& position, const Vec2& direction,
               double fov, double nearDistance, double drawDistance) : 
               map(map),
               position(position),
               nearDistance(nearDistance),
               drawDistance(drawDistance)
{
    zUnit = direction;
    zUnit.normalize();
    
    xUnit = direction;
    xUnit.rotate(-M_PI/2);
    xUnit.normalize();
    
    viewRadius = nearDistance * sin(fov/2);
}


void Camera::moveX(double dx)
{
    position += dx * xUnit;
}

void Camera::moveZ(double dz)
{
    position += dz * zUnit;
}

void Camera::rotate(double angle)
{
    xUnit.rotate(angle);
    zUnit.rotate(angle);
}


void Camera::render(SDL_Surface* surface) const
{
    // fill in the background
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format,
                                           SKY_COLOR.r,
                                           SKY_COLOR.g,
                                           SKY_COLOR.b));
    
    // walls reach equally above and below horizon
    const int HORIZON = surface->h / 2; // horizon in center of screen
    const int WALL_HEIGHT = surface->h; // walls touch top and bottom of screen at z=1
    
    // For each column of the image
    for (int x = 0; x < surface->w; x++)
    {
        // Cast a ray
        Ray ray = Ray(position, findColumn(x, surface->w) - position);
        Vec2 intersection = map->cast(ray, drawDistance);

        // If the ray intersects a wall, draw it
        if (!map->passable(intersection))
        {
            // find visible height of wall
            double z = dot(intersection - position, zUnit);
                       
            SDL_Rect column =
            {
                x,                              // x
                HORIZON - (WALL_HEIGHT/2) / z,  // y
                1,                              // width
                WALL_HEIGHT / z                 // height
            };
                       
            // add fog so we can't see walls clip out of existence
            int shade = 255 * z / drawDistance;
            
            SDL_Color shaded = WALL_COLOR;
            shaded.r = shaded.r > shade ? shaded.r - shade : 0;
            shaded.g = shaded.g > shade ? shaded.g - shade : 0;
            shaded.b = shaded.b > shade ? shaded.b - shade : 0;
            
            // draw wall
            column.x = x;
            SDL_FillRect(surface, &column, SDL_MapRGB(surface->format,
                                                      shaded.r,
                                                      shaded.g,
                                                      shaded.b));
        }
    }
}


// TODO function to convert camera to world coords
Vec2 Camera::findColumn(int x, int w) const
{
    // Origin to center of image
    Vec2 center = position + nearDistance * zUnit;
    
    // Center point to pixel
    Vec2 cameraX = ((2 * (double)x / w) - 1) * viewRadius * xUnit;

    return center + cameraX;
}
