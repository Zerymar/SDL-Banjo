#pragma once

class Vector2
{
public:
    float x, y;
    Vector2();
    Vector2(float x, float y);

    Vector2 operator-=(Vector2 const& vector);
    Vector2 operator-(Vector2 const& vector) const;
    Vector2 operator+=(Vector2 const& vector);
    Vector2 operator+(Vector2 const& vector) const;
    Vector2 operator*=(Vector2 const& vector);
    Vector2 operator*(Vector2 const& vector) const;
    Vector2 operator*=(float const& scalar);
    Vector2 operator*(float const& scalar) const;

    void normalize();
    static float dot_product(Vector2 const& vectorA, Vector2 const& vectorB);
    
};
