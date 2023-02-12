#include "Vector2.h"


Vector2::Vector2()
{
    x = 0;
    y = 0;
}

Vector2::Vector2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2 Vector2::operator+(Vector2 const& vector) const
{
    return Vector2(
            x + vector.x,
            y + vector.y);
}



Vector2 Vector2::operator+=(Vector2 const& vector)
{
    x += vector.x;
    y += vector.y;

    return *this;
}

Vector2 Vector2::operator-(Vector2 const& vector) const
{
    return Vector2(
    x - vector.x,
    y - vector.y);
}

Vector2 Vector2::operator-=(Vector2 const& vector)
{
    x -= vector.x;
    y -= vector.y;

    return *this;
}
Vector2 Vector2::operator*=(Vector2 const& vector)
{
    x *= vector.x;
    y *= vector.y;

    return *this;
}

Vector2 Vector2::operator*(Vector2 const& vector) const
{
    return Vector2(
        x * vector.x,
        y * vector.y);
}

Vector2 Vector2::operator*=(float const& scalar)
{
    x *= scalar;
    y *= scalar;

    return *this;
}

Vector2 Vector2::operator*(float const& scalar ) const
{
    return Vector2(
    x * scalar,
    y * scalar);
}

float Vector2::dot_product(Vector2 const& vectorA, Vector2 const& vectorB)
{
    return (vectorA.x * vectorB.x) + (vectorA.y * vectorB.y);
}
