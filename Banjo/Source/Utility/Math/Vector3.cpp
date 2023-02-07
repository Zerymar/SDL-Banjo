#include "Vector3.h"

Vector3::Vector3()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}
Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3 Vector3::operator+(Vector3 const& vector) const
{
    return Vector3(x+vector.x,
        y+vector.y,
        z+vector.z);
}

Vector3 Vector3::operator+=(Vector3 const& vector)
{
    x += vector.x;
    y += vector.y;
    z += vector.z;
    
    return *this;
}

Vector3 Vector3::operator-(Vector3 const& vector) const
{
    return Vector3(x-vector.x,
        y-vector.y,
        z-vector.z);
}

Vector3 Vector3::operator-=(Vector3 const& vector)
{
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
    
    return *this;
}

Vector3 Vector3::operator*(Vector3 const& vector) const
{
    return Vector3(x*vector.x,
        y*vector.y,
        z*vector.z);
}

Vector3 Vector3::operator*=(Vector3 const& vector)
{
    x *= vector.x;
    y *= vector.y;
    z *= vector.z;
    
    return *this;
}

Vector3 Vector3::operator*(float scalar) const
{
    return Vector3(x*scalar,
        y*scalar,
        z*scalar);
}

Vector3 Vector3::operator*=(float scalar) 
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    
    return *this;
}

float Vector3::dot_product(Vector3 const& vectorA, Vector3 const& vectorB)
{
    return (vectorA.x * vectorB.x) + (vectorA.y * vectorB.y) + (vectorA.z * vectorB.z); 
}

Vector3 Vector3::cross_product(Vector3 const& vectorA, Vector3 const& vectorB)
{
    float x = vectorA.y*vectorB.z - vectorA.z*vectorB.y;
    float y = vectorA.z*vectorB.x - vectorA.x*vectorB.z;
    float z = vectorA.x*vectorB.y - vectorA.y*vectorB.x;
    return Vector3(x,y,z);
}
