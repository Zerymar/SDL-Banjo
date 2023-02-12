#pragma once

//Right hand side
class Vector3
{
public:
    Vector3();
    Vector3(float x,  float y,  float z);

    Vector3 operator+(Vector3 const& vector) const;
    Vector3 operator +=(Vector3 const& vector);

    Vector3 operator-(Vector3 const& vector) const;
    Vector3 operator -=(Vector3 const& vector);

    Vector3 operator*(Vector3 const& vector) const;
    Vector3 operator *=(Vector3 const& vector);

    Vector3 operator*(int scalar) const;
    Vector3 operator*=(int scalar);

    static float dot_product(Vector3 const& vectorA, Vector3 const& vectorB);
    static Vector3 cross_product(Vector3 const& vectorA, Vector3 const& vectorB);

    int x, y, z;
};
