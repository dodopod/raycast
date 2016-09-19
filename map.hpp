#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include "ray.hpp"
#include "vec2.hpp"

typedef std::vector<bool> MapRow;
typedef std::vector<MapRow> MapGrid;

class Map
{
public:
    Map(const MapGrid& grid);
    
    int width() const;
    int height() const;
    
    // Returns true if point is inside map.
    bool inside(const Vec2& point) const;
    
    // Returns true if point isn't inside wall.
    bool passable(const Vec2& point) const;
    
    // Returns point where ray intersects a wall, or (-1, -1) if it doesn't.
    Vec2 cast(const Ray& ray, double range) const;
    
private:
    MapGrid grid;
    
    bool isWhole(double x) const;
};

#endif
