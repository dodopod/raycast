#include "ray.hpp"

Ray::Ray(const Vec2& p, const Vec2& v) : origin(p), direction(v)
{
    direction.normalize();
}

Vec2 Ray::getOrigin() const
{
    return origin;
}

Vec2 Ray::getDirection() const
{
    return direction;
}

void Ray::setOrigin(const Vec2& p)
{
    origin = p;
}

void Ray::setDirection(const Vec2& v)
{
    direction = v;
    direction.normalize();
}
