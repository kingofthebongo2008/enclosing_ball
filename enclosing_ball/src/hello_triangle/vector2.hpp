namespace computational_geometry
{
    inline vector2 operator-(const point2 a, const point2 b)
    {
        return { a.m_x - b.m_x, a.m_y - b.m_y};
    }

    inline vector2 operator*(const vector2 v, float scalar)
    {
        return { v.m_x * scalar, v.m_y * scalar};
    }

    inline vector2 operator*(float scalar, const vector2 v)
    {
        return { v.m_x * scalar, v.m_y * scalar };
    }

    inline point2 operator+(const point2 a, const vector2 b)
    {
        return { a.m_x + b.m_x, a.m_y + b.m_y };
    }

    inline float dot2(const vector2 a, const vector2 b)
    {
        return (a.m_x * b.m_x) + (a.m_y * b.m_y);
    }

    inline float norm2(const vector2 a)
    {
        return sqrtf(dot2(a, a));
    }

    inline vector2 normalize2(const vector2 a)
    {
        return a * (1.0f / norm2(a));
    }
}