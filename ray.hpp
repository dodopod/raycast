#ifndef RAY_HPP
#define RAY_HPP

#include "vec2.hpp"

class Ray
{
public:
    Ray(const Vec2& p, const Vec2& v);
    
    Vec2 getOrigin() const;
    Vec2 getDirection() const;
    
    void setOrigin(const Vec2& p);
    void setDirection(const Vec2& v);
    
private:
    Vec2 origin;
    Vec2 direction;
};

#endif
