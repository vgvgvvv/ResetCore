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


    Vector3 normalize(){
        float magSq = x*x + y*y + z*z;
        if(magSq > 0){
            float oneOverMag = static_cast<float>(1.0 / sqrt(magSq));
            return Vector3(x * oneOverMag, y * oneOverMag, z * oneOverMag);
        }
        return Vector3(x, y, z);
    }

    float dot(const Vector3& other) const{
        return x * other.x + y * other.y + z * other.z;
    }

    float magnitude() const {
        return sqrt(x*x + y*y + z*z);
    }
};


#endif //REMATH_VECTOR3_HPP
