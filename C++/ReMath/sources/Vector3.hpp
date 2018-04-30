//
// Created by 董宸 on 2018/4/12.
//

#ifndef REMATH_VECTOR3_HPP
#define REMATH_VECTOR3_HPP

#include <cmath>

/**
 * Vector3
 * 基于右手坐标系
 */
class Vector3 {

public:
    float x, y, z;


    Vector3() : x(0), y(0), z(0){}
    Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
    Vector3(const Vector3& vector3) = default;

    Vector3 &operator=(const Vector3 &other) = default;

    bool operator==(const Vector3 &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vector3 &other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    Vector3 operator -(){
        return Vector3(-x, -y, -z);
    }

    Vector3 operator + (const Vector3 other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 operator - (const Vector3 other) const {
        return Vector3(x-other.x, y-other.y, z-other.z);
    }

    Vector3 operator * (const float ratio) const {
        return Vector3(x * ratio, y * ratio, z * ratio);
    }

    Vector3 operator / (const float ratio) const{
        return Vector3(x/ratio, y/ratio, z/ratio);
    }

    Vector3& operator -= (const Vector3 vector3) {
        x -= vector3.x;
        y -= vector3.y;
        z -= vector3.z;
        return *this;
    }

    Vector3&operator += (const Vector3 vector3){
        x += vector3.x;
        y += vector3.y;
        z += vector3.z;
        return *this;
    }

    Vector3&operator *= (const float ratio){
        x *= ratio;
        y *= ratio;
        z *= ratio;
        return *this;
    }

    Vector3&operator /= (const float ratio){
        x /= ratio;
        y /= ratio;
        z /= ratio;
        return *this;
    }


    Vector3 normalize() const {
        float magSq = x*x + y*y + z*z;
        if(magSq > 0){
            float oneOverMag = static_cast<float>(1.0 / sqrt(magSq));
            return Vector3(x * oneOverMag, y * oneOverMag, z * oneOverMag);
        }
        return Vector3(x, y, z);
    }

    Vector3 crossProduct(const Vector3& other) const{
        return Vector3(y*other.z - z*other.y, x*other.z - z*other.x, x*other.y - y*other.x);
    }

    float dot(const Vector3& other) const{
        return x * other.x + y * other.y + z * other.z;
    }

    float magnitude() const {
        return (float) sqrt(x * x + y * y + z * z);
    }

    float distance(const Vector3& other) const{
        auto disX = x - other.x;
        auto disY = y - other.y;
        auto disZ = z - other.z;
        return (float) sqrt(disX * disX + disY * disY + disZ * disZ);
    }
};


#endif //REMATH_VECTOR3_HPP
