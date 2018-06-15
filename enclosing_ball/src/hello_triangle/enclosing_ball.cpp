#include "pch.h"
#include <cstdint>
#include <intrin.h>

#include "enclosing_ball.h"
#include "matrix22.h"
#include "vector2.h"
#include "vector3.h"

#include "matrix22.h"
#include "matrix33.h"

namespace computational_geometry
{
    const float radius_epsilon = 1e-4f;

    sphere make_sphere(const point3 a)
    {
        sphere s;
        s.m_center = a;
        s.m_radius = radius_epsilon;
        return s;
    }

    sphere make_sphere(const point3 a, const point3 b)
    {
        sphere s;
        vector3  v = (b - a) * 0.5f;
        s.m_center = a + v;
        s.m_radius = norm3(v) + radius_epsilon;
        return s;
    }

    sphere make_sphere(const point3 a, const point3 b, const point3 c)
    {
        sphere s;
        s.m_center = a;
        return s;
    }

    sphere make_sphere(const point3 a, const point3 b, const point3 c, const point3 d)
    {
        sphere s;
        s.m_center = a;
        return s;
    }

    template <typename iterator> point3 farthest_point(const sphere&s, const point3_soa& points)
    {
        return point3();
    }

    sphere enclosing_ball(std::vector<point3>& points)
    {
        sphere r;
        return r;
    }

    void test2()
    {

    }
}
