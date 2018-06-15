#include "invert_2x2_c.h"

namespace computational_geometry
{
    inline vector2 row0(const matrix22& m)
    {
        return { m.m_separate.v00,m.m_separate.v01};
    }

    inline vector2 row1(const matrix22& m)
    {
        return { m.m_separate.v10,m.m_separate.v11};
    }

    inline vector2 column0(const matrix22& m)
    {
        return { m.m_separate.v00,m.m_separate.v10};
    }

    inline vector2 column1(const matrix22& m, uint32_t r)
    {
        return { m.m_separate.v01,m.m_separate.v11};
    }

    inline matrix22 identity_matrix_22()
    {
        return
        {
            1,0,
            0,1
        };
    }

    inline matrix22 invert(const matrix22& m)
    {
        matrix22 r;
        invert2x2(&m.m_separate.v00, &r.m_separate.v00);
        return r;
    }
}