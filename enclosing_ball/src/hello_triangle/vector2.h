#pragma once

namespace computational_geometry
{
    struct point2
    {
        float m_x = 0.0f;
        float m_y = 0.0f;
    };

    struct vector2
    {
        float m_x = 0.0f;
        float m_y = 0.0f;
    };

    vector2 operator-(const point2 a, const point2 b);
    vector2 operator*(const vector2 v, float scalar);
    vector2 operator*(float scalar, const vector2 v);
    point2 operator+(const point3 a, const vector2 b);
    float dot2(const vector2 a, const vector2 b);
    float norm2(const vector2 a);
    vector2 normalize2(const vector2 a);
}

#include "vector2.hpp"