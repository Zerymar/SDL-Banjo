#include "Vector2.h"

#include <complex>


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

bool Vector2::operator<(Vector2 const& vector) const
{
    return x < vector.x & y < vector.y;
}
bool Vector2::operator<=(Vector2 const& vector) const
{
    return x <= vector.x & y <= vector.y;
}

bool Vector2::operator>(Vector2 const& vector) const
{
    return x > vector.x & y > vector.y;
}

bool Vector2::operator>=(Vector2 const& vector) const
{
    return x >= vector.x & y >= vector.y;
}


Vector2 Vector2::operator+=(Vector2 const& vector)
{
    x += vector.x;
    y += vector.y;

    return *this;
}

Vector2 Vector2::operator+=(float const& scalar)
{
    return Vector2(x + scalar,y + scalar);
}
Vector2 Vector2::operator-=(float const& scalar)
{
    return Vector2(x - scalar,y - scalar);
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

void Vector2::normalize()
{
    float magnitude = std::sqrt(pow(x, 2) + pow(y ,2));
    this->x = this->x / magnitude;
    this->y = this->y / magnitude;
}

float Vector2::dot_product(Vector2 const& vectorA, Vector2 const& vectorB)
{
    return (vectorA.x * vectorB.x) + (vectorA.y * vectorB.y);
}
