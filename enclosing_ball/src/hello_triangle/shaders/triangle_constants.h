#if defined(__cplusplus)

#include <DirectXMath.h>

typedef DirectX::XMFLOAT4X4A    float4x4;
typedef DirectX::XMFLOAT4A      float4;
typedef DirectX::XMINT4         int4;

#endif

struct frame_constants
{
    float4x4 m_view;
    float4x4 m_perspective;
    float4x4 m_world;
};

float4x4 get_view(frame_constants s)
{
    return s.m_view;
}

float4x4 get_perspective(frame_constants s)
{
    return s.m_perspective;
}


float4x4 get_world(frame_constants s)
{
    return s.m_world;
}

struct sphere_constants
{
    float4 m_sphere_radius;
    int4   m_subdivision_count;
};

float get_sphere_radius(sphere_constants s)
{
    return s.m_sphere_radius.x;
}

float get_sphere_subdivision_count(sphere_constants s )
{
    return s.m_subdivision_count.x;
}

