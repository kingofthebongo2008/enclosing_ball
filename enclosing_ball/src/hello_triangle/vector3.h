#pragma once

namespace computational_geometry
{
    struct vector3
    {
        float m_x = 0.0f;
        float m_y = 0.0f;
        float m_z = 0.0f;
    };

    vector3 operator-(const point3 a, const point3 b);
    vector3 operator*(const vector3 v, float scalar);
    vector3 operator*(float scalar, const vector3 v);
    point3 operator+(const point3 a, const vector3 b);
    float dot3(const vector3 a, const vector3 b);
    float norm3(const vector3 a);
    vector3 cross3(const vector3 a, const vector3& b);
    vector3 normalize3(const vector3 a);
}

#include "vector3.hpp"