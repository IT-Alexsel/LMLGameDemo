#pragma once
// 二维向量

#include <cmath>


class Vector2
{
public:
    double x;
    double y;

    Vector2() = default;
    ~Vector2() = default;

    Vector2(double x, double y)
        : x(x), y(y) {};

    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const
    {
        return Vector2(x - other.x, y - other.y);
    };

    void operator+=(const Vector2& other) 
    {
        x += other.x;
        y += other.y;
    }

    void operator-=(const Vector2& other)
    {
        x -= other.x;
        y -= other.y;
    }

    Vector2 operator*(double scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }

    void operator*=(double scalar)
    {
        x *= scalar;
        y *= scalar;
    }

    double operator*(const Vector2& other) const
    {
        return x * other.x + y * other.y;
    }

    bool operator==(const Vector2& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2& other) const
    {
        return x != other.x || y != other.y;
    }

    bool operator<(const Vector2& other) const
    {
        return Length() < other.Length();
    }
    bool operator>(const Vector2& other) const
    {
        return Length() > other.Length();
    }

    double Length() const
    { 
        return sqrt(x * x + y * y);
    };

    Vector2 Normalize() const 
    {
        double len = Length();
        if (len == 0)
            return Vector2(0, 0);
        return Vector2(x / len, y / len);
    }

    bool Approx_Zero() const
    {
        return Length() < 0.0001;
    }

};