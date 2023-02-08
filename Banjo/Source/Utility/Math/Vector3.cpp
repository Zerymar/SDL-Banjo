#include "Vector3.h"

Vector3::Vector3()
{
    x = 0;
    y = 0;
    z = 0;
}
Vector3::Vector3(int x, int y, int z)
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

Vector3 Vector3::operator*(int scalar) const
{
    return Vector3(x*scalar,
        y*scalar,
        z*scalar);
}

Vector3 Vector3::operator*=(int scalar) 
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
