#pragma once

#include <vector>

namespace computational_geometry
{
    struct point3
    {
        float m_x = 0.0f;
        float m_y = 0.0f;
        float m_z = 0.0f;
    };

    struct sphere
    {
        point3 m_center;
        float  m_radius = 0.0f;;
    };


    sphere enclosing_ball(const std::vector<point3>& points);

}
