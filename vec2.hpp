#ifndef VEC2_HPP
#define VEC2_HPP

#include <cmath>

struct Vec2
{
    Vec2(double x=0, double y=0);
    
    double x;
    double y;
    
    // Returns magnitude of vector
    double getMagnitude() const;
    
    // Rotates vector CCW by an angle given in radians
    void rotate(double angle);
    
    // Sets vector magnitude to 1
    void normalize();
};


Vec2 operator-(const Vec2& v);

Vec2 operator+(const Vec2& v, const Vec2& u);
Vec2 operator-(const Vec2& v, const Vec2& u);
Vec2 operator*(double a, const Vec2& v);

void operator+=(Vec2& v, const Vec2& u);
void operator/=(Vec2& v, double a);


// Returns dot product of 2 vectors
double dot(const Vec2& v, const Vec2& u);

// Returns Euclidean distance between 2 points
double distance(const Vec2& p1, const Vec2& p2);

#endif
