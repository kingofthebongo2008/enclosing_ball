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

    struct point3_soa
    {
        std::vector<float> m_x;
        std::vector<float> m_y;
        std::vector<float> m_z;
    };


    sphere enclosing_ball(point3_soa& points);

}
