#pragma once

class Vector2
{
public:
    int x, y;
    Vector2();
    Vector2(int x, int y);

    Vector2 operator-=(Vector2 const& vector);
    Vector2 operator-(Vector2 const& vector) const;
    Vector2 operator+=(Vector2 const& vector);
    Vector2 operator+(Vector2 const& vector) const;
    Vector2 operator*=(Vector2 const& vector);
    Vector2 operator*(Vector2 const& vector) const;
    Vector2 operator*=(int const& scalar);
    Vector2 operator*(int const& scalar) const;

    static float dot_product(Vector2 const& vectorA, Vector2 const& vectorB);
    
};
