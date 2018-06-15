#include "invert_3x3_c.h"

namespace computational_geometry
{
    inline vector3 row0(const matrix33& m)
    {
        return { m.m_separate.v00,m.m_separate.v01,m.m_separate.v02 };
    }

    inline vector3 row1(const matrix33& m)
    {
        return { m.m_separate.v10,m.m_separate.v11,m.m_separate.v12 };
    }

    inline vector3 row2(const matrix33& m)
    {
        return { m.m_separate.v20,m.m_separate.v21,m.m_separate.v22 };
    }

    inline vector3 column0(const matrix33& m)
    {
        return { m.m_separate.v00,m.m_separate.v10,m.m_separate.v20 };
    }

    inline vector3 column1(const matrix33& m, uint32_t r)
    {
        return { m.m_separate.v01,m.m_separate.v11,m.m_separate.v21 };
    }

    inline vector3 column2(const matrix33& m, uint32_t r)
    {
        return { m.m_separate.v02,m.m_separate.v12,m.m_separate.v22 };
    }

    inline matrix33 identity_matrix_33()
    {
        return
        {
            1,0,0,
            0,1,0,
            0,0,1
        };
    }

    inline matrix33 invert(const matrix33& m)
    {
        matrix33 r;
        invert3x3(&m.m_separate.v00, &r.m_separate.v00);
        return r;
    }
}