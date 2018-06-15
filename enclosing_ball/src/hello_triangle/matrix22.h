#pragma once

#include "vector2.h"

namespace computational_geometry
{
    struct matrix22
    {
        struct separate
        {
            float v00;
            float v01;

            float v10;
            float v11;
        };

        struct vector2
        {
            float x;
            float y;
        };

        struct simd
        {
            vector2 m_simd[2];
        };

        union
        {
            separate m_separate;
            simd     m_simd;
        };
    };

    vector2     row0(const matrix22& m);
    vector2     row1(const matrix22& m);
    vector2     column0(const matrix22& m);
    vector2     column1(const matrix22& m, uint32_t r);
    matrix22    identity_matrix_22();
    matrix22    invert(const matrix22& m);
}

#include "matrix22.hpp"