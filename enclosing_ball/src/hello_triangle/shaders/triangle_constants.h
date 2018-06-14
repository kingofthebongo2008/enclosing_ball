#ifndef __triangle_constants__
#define __triangle_constants__

#if defined(__cplusplus)

#include <DirectXMath.h>

typedef DirectX::XMFLOAT4X4A    float4x4;
typedef DirectX::XMFLOAT4A      float4;
typedef DirectX::XMINT4         int4;
typedef DirectX::XMFLOAT3       float3;

#endif

struct frame_constants
{
    float4x4 m_view;
    float4x4 m_perspective;
};

struct object_constants
{
    float4x4 m_world;
};

struct sphere_constants
{
    float4 m_sphere_parameters; //x sphere radius, y subdivision count 
};

struct spheres
{
    float4 m_spheres[4096];
};


#if !defined(__cplusplus)

float4x4 get_view(frame_constants s)
{
    return s.m_view;
}

float4x4 get_perspective(frame_constants s)
{
    return s.m_perspective;
}

float4x4 get_world(object_constants s)
{
    return s.m_world;
}

float get_sphere_radius(sphere_constants s)
{
    return s.m_sphere_parameters.x;
}

int get_sphere_subdivision_count(sphere_constants s )
{
    return (int)s.m_sphere_parameters.y;
}

//todo: move this out
static const float pi = 3.141592654f;
static const float two_pi = 6.283185307f;
static const float pi_div_two = 1.570796327f;

float get_sphere_radius(spheres s, int sphere)
{
    return s.m_spheres[sphere].w;
}

float3 get_sphere_position(spheres s, int sphere)
{
    return s.m_spheres[sphere].xyz;
}
#endif

#endif