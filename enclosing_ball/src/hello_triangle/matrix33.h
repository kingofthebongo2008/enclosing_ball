#pragma once

#include "vector3.h"

namespace computational_geometry
{
    struct matrix33
    {
        struct separate
        {
            float v00;
            float v01;
            float v02;

            float v10;
            float v11;
            float v12;

            float v20;
            float v21;
            float v22;
        };

        struct vector3
        {
            float x;
            float y;
            float z;
        };

        struct simd
        {
            vector3 m_simd[3];
        };

        union
        {
            separate m_separate;
            simd     m_simd;
        };
    };

    vector3 row0(const matrix33& m);
    vector3 row1(const matrix33& m);
    vector3 row2(const matrix33& m);
    vector3 column0(const matrix33& m);
    vector3 column1(const matrix33& m, uint32_t r);
    vector3 column2(const matrix33& m, uint32_t r);
    matrix33 identity_matrix_33();
    matrix33 invert(const matrix33& m);
}

#include "matrix33.hpp"