namespace computational_geometry
{
    inline vector3 operator-(const point3 a, const point3 b)
    {
        return { a.m_x - b.m_x, a.m_y - b.m_y, a.m_z - b.m_z };
    }

    inline vector3 operator*(const vector3 v, float scalar)
    {
        return { v.m_x * scalar, v.m_y * scalar, v.m_z * scalar };
    }

    inline vector3 operator*(float scalar, const vector3 v)
    {
        return { v.m_x * scalar, v.m_y * scalar, v.m_z * scalar };
    }

    inline point3 operator+(const point3 a, const vector3 b)
    {
        return { a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z };
    }

    inline float dot3(const vector3 a, const vector3 b)
    {
        return (a.m_x * b.m_x) + (a.m_y * b.m_y) + (a.m_z * b.m_z);
    }

    inline float norm3(const vector3 a)
    {
        return sqrtf(dot3(a, a));
    }

    inline vector3 cross3(const vector3 a, const vector3& b)
    {
        float u1 = a.m_x;
        float u2 = a.m_y;
        float u3 = a.m_z;

        float v1 = b.m_x;
        float v2 = b.m_y;
        float v3 = b.m_z;

        float x = (u2 * v3) - (u3* v2);
        float y = (u3 * v1) - (u1* v3);
        float z = (u1 * v2) - (u2* v1);

        return { x,y,z };
    }

    inline vector3 normalize3(const vector3 a)
    {
        return a * (1.0f / norm3(a));
    }
}