#pragma once
namespace computational_geometry
{
    struct float4
    {
        struct separate
        {
            float m_x;
            float m_y;
            float m_z;
            float m_w;
        };

        union
        {
            __m128      m_simd;
            separate    m_separate;
        } m_value;
    };

    float x(float4 v)
    {
        return v.m_value.m_separate.m_x;
    }

    float y(float4 v)
    {
        return v.m_value.m_separate.m_y;
    }

    float z(float4 v)
    {
        return v.m_value.m_separate.m_z;
    }

    float w(float4 v)
    {
        return v.m_value.m_separate.m_w;
    }

    struct matrix44
    {
        struct separate
        {
            float v00;
            float v01;
            float v02;
            float v03;

            float v10;
            float v11;
            float v12;
            float v13;

            float v20;
            float v21;
            float v22;
            float v23;

            float v30;
            float v31;
            float v32;
            float v33;
        };

        struct simd
        {
            float4 m_r[4];
        };

        union
        {
            separate m_separate;
            simd     m_simd;
        };
    };

}

#include "matrix33.hpp"