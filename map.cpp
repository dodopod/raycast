#include "map.hpp"

Map::Map(const MapGrid& grid) : grid(grid)
{}


int Map::width() const
{
    return grid[0].size();
}

int Map::height() const
{
    return grid.size();
}


bool Map::inside(const Vec2& point) const
{
    return point.x >= 0 && point.x < width() && point.y >=0 && point.y < height();
}

bool Map::passable(const Vec2& point) const
{
    // if point is on an edge, we should check both sides
    bool edge = false;
    Vec2 nudge;
    if (isWhole(point.x))
    {
        nudge.x = 0.1;
        edge = true;
    }
    if (isWhole(point.y))
    {
        nudge.y = 0.1;
        edge = true;
    }
    
    if (edge)
    {
        return passable(point - nudge) && passable(point + nudge);
    }
    else if (inside(point))
    {
        return !grid[point.y][point.x];
    }
    else
    {
        return true;
    }
}

Vec2 Map::cast(const Ray& ray, double range) const
{
    Vec2 point = ray.getOrigin();
    Vec2 nudge;
    
    // step the ray forward until we hit a wall or our max range
    while (passable(point + nudge) && distance(ray.getOrigin(), point) <= range)
    {
        double slope = ray.getDirection().y / ray.getDirection().x;
        
        // find position at next whole X
        Vec2 stepX;
        stepX.x = ray.getDirection().x < 0 ? ceil(point.x - 1) : floor(point.x + 1);
        stepX.y = (stepX.x - point.x) * slope + point.y;
        
        // find position at next whole Y
        Vec2 stepY;
        stepY.y = ray.getDirection().y < 0 ? ceil(point.y - 1) : floor(point.y + 1);
        stepY.x = (stepY.y - point.y) / slope + point.x;
        
        // move position to closer of the two
        point = distance(point, stepX) < distance(point, stepY) ? stepX : stepY;
    }
    
    return point;
}

bool Map::isWhole(double x) const
{
    return x == (int)x;
}
