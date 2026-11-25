#include "headersrc/Vector3.h"
#include <cmath>
#include <random>

namespace Lumis{

    // Operator OverLoading
    Vector3 Vector3::operator+(const Vector3& other) const{
        return {v_x + other.v_x, v_y + other.v_y, v_z + other.v_z};
    }

    Vector3 Vector3::operator-(const Vector3& other) const{
        return {v_x - other.v_x, v_y - other.v_y, v_z - other.v_z};
    }

    Vector3 Vector3::operator-() const{
        return {-v_x, -v_y, -v_z};
    }

    Vector3 Vector3::operator*(const Vector3& other) const{
        return {v_x * other.v_x, v_y * other.v_y, v_z * other.v_z};
    }

    Vector3 Vector3::operator*(const float scalar) const{
        return {v_x * scalar, v_y * scalar, v_z * scalar};
    }

    Vector3 Vector3::operator/(const float scalar) const{
        if (scalar != 0){
            const float invScalar = 1 / scalar;
            return (*this) * invScalar;

        }
        return {0, 0, 0};
    }

    bool Vector3::operator==(const Vector3& other) const{
        return (fabsf(v_x - other.v_x) < (0.00000001) &&
            fabsf(v_y - other.v_y) < (0.00000001) &&
            fabsf(v_z - other.v_z) < (0.00000001));
    }

    float Vector3::operator[](const int index) const{
        if (index >= 0 && index <= 2){
            switch (index){
            case 0: return v_x;
            case 1: return v_y;
            case 2: return v_z;
            default: return INFINITY;
            }
        }
        return INFINITY;
    }

    // Cross and Dot Products
    float Vector3::dotProduct(const Vector3& other) const{
        return (v_x * other.v_x + v_y * other.v_y + v_z * other.v_z);
    }

    Vector3 Vector3::crossProduct(const Vector3& other) const{
        return {v_y * other.v_z - v_z * other.v_y, v_z * other.v_x - v_x * other.v_z, v_x * other.v_y - v_y * other.v_x};
    }

    //
    float Vector3::magnitude() const{
        return std::sqrtf(magnitudeSq());
    }

    float Vector3::magnitudeSq() const{
        return (v_x * v_x + v_y * v_y + v_z * v_z);
    }

    void Vector3::normalize(){
        const float mag = magnitude();
        if (mag > 0){
            v_x /= mag; v_y /= mag; v_z /= mag;
        }
    }

    Vector3 Vector3::normalized() const{
        const float mag = magnitude();
        if (mag > 0){
            return {v_x / mag, v_y / mag, v_z / mag};
        }
        return {0, 0, 0};
    }

    bool Vector3::near_zero() const{
        constexpr float eps = 1e-6f;
        return (std::fabs(v_x) < eps) && (std::fabs(v_y) < eps) && (std::fabs(v_z) < eps);
    }

    Vector3 Vector3::reflectRay(const Vector3& vec, const Vector3& normal){

        Vector3 normalv = normal.normalized();
        return vec - normalv * vec.dotProduct(normalv) * 2;
    }

    Vector3 Vector3::refract(const Vector3& uv, const Vector3& normal, const float etaiOverEtat){
        const float cosTheta = std::fmin((-uv).dotProduct(normal), 1.0f);
        const Vector3 rOutPerp = etaiOverEtat * (uv + cosTheta * normal);
        const Vector3 rOutParallel = -std::sqrt(std::fabs(1.0f - rOutPerp.magnitudeSq())) * normal;
        return rOutPerp + rOutParallel;
    }

    Vector3 Vector3::random(){
        return {randomFloat(), randomFloat(), randomFloat()};
    }

    Vector3 Vector3::random(const float min, const float max){
        return {randomFloat(min, max), randomFloat(min, max), randomFloat(min, max)};
    }

    Vector3 Vector3::randomInUnitSphere(){
        while (true){
            const Vector3 p = Vector3::random(-1.0f, 1.0f);
            if (p.magnitudeSq() < 1.0f){
                return p;
            }
        }
    }

    Vector3 Vector3::randomUnitVector(){
        while (true){
            const Vector3 p = Vector3::random(-1.0f, 1.0f);
            const float lenSq = p.magnitudeSq();
            if (lenSq > 1e-12f && lenSq <= 1.0f){
                return p / std::sqrt(lenSq);
            }
        }
    }

    Vector3 operator*(const float scalar, const Vector3& vec){
        return vec * scalar;
    }

    float randomFloat(){
        thread_local std::mt19937 generator(std::random_device{}());
        thread_local std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
        return distribution(generator);
    }

    float randomFloat(const float min, const float max){
        return min + (max - min) * randomFloat();
    }
}
