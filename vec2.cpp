#include "vec2.hpp"

Vec2::Vec2(double x, double y) : x(x), y(y)
{}


double Vec2::getMagnitude() const
{
    return sqrt(x * x + y * y);
}


void Vec2::rotate(double angle)
{
    double oldX = x;
    
    x = x * cos(angle) + y * sin(angle);
    y = oldX * -sin(angle) + y * cos(angle);
}

void Vec2::normalize()
{
    *this /= this->getMagnitude();
}


Vec2 operator-(const Vec2& v)
{
    return Vec2(-v.x, -v.y);
}


Vec2 operator+(const Vec2& v, const Vec2& u)
{
    return Vec2(v.x + u.x, v.y + u.y);
}

Vec2 operator-(const Vec2& v, const Vec2& u)
{
    return Vec2(v.x - u.x, v.y - u.y);
}

Vec2 operator*(double a, const Vec2& v)
{
    return Vec2(a * v.x, a * v.y);
}


void operator+=(Vec2& v, const Vec2& u)
{
    v.x += u.x;
    v.y += u.y;
}

void operator/=(Vec2& v, double a)
{
    v.x /= a;
    v.y /= a;
}


double dot(const Vec2& v, const Vec2& u)
{
    return v.x * u.x + v.y * u.y;
}

double distance(const Vec2& p1, const Vec2& p2)
{
    return (p2 - p1).getMagnitude();
}
