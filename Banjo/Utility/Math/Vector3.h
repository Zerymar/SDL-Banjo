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

    Vector3 operator*(float scalar) const;
    Vector3 operator*=(float scalar) ;

    float x, y, z;
};
