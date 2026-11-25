#ifndef RAYTRACING_VECTOR3_H
#define RAYTRACING_VECTOR3_H


namespace Lumis{
    struct Vector3{

        union{
            struct{ float v_x, v_y, v_z; };
            struct{ float v_r, v_g, v_b; };
        };

        // Constructors
        Vector3(const float x = 0, const float y = 0, const float z = 0): v_x(x), v_y(y), v_z(z) {}
        Vector3(const Vector3& other): v_x(other.v_x), v_y(other.v_y), v_z(other.v_z) {}

        // Operator OverLoading
        Vector3 operator+(const Vector3& other) const;
        Vector3 operator-(const Vector3& other) const;
        Vector3 operator-() const;
        Vector3 operator*(const Vector3& other) const;
        Vector3 operator*(float scalar) const;
        Vector3 operator/(float scalar) const;

        bool operator==(const Vector3& other) const;
        float operator[](const int index) const;

        // Cross and Dot Products;
        float dotProduct(const Vector3& other) const;
        Vector3 crossProduct(const Vector3& other) const;

        //
        float magnitude() const;
        float magnitudeSq() const;
        void normalize();
        Vector3 normalized() const;
        bool near_zero() const;
        static Vector3 reflectRay(const Vector3& vec, const Vector3& normal);
        static Vector3 refract(const Vector3& uv, const Vector3& normal, float etaiOverEtat);

        static Vector3 random();
        static Vector3 random(float min, float max);
        static Vector3 randomInUnitSphere();
        static Vector3 randomUnitVector();

    };

    Vector3 operator*(float scalar, const Vector3& vec);
    float randomFloat();
    float randomFloat(float min, float max);
}


#endif //RAYTRACING_VECTOR3_H
