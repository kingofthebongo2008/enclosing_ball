#include "triangle_constants.h"

#include "draw_sphere.hlsli"

struct interpolated_value
{
    float4 m_position : SV_POSITION;
};

cbuffer g_frame_constants_buffer_cb     : register(b0) { frame_constants     g_frame_constants; };
cbuffer g_sphere_constants_buffer_cb    : register(b1) { sphere_constants    g_sphere_constants; };
cbuffer g_sphere_buffer_cb              : register(b2) { spheres             g_spheres; };

float4x4 translation_matrix(float3 position)
{
    float4x4 r;

    r[0] = float4(1.0, 0.0, 0.0, 0.0f);
    r[1] = float4(0.0, 1.0, 0.0, 0.0f);
    r[2] = float4(0.0, 0.0, 1.0, 0.0f);
    r[3] = float4(position.x, position.y, position.z, 1.0f);

    return r;
}

interpolated_value main(uint v : SV_VERTEXID, uint instance : SV_INSTANCEID)
{
    interpolated_value r;
    float radius                    = get_sphere_radius(g_spheres,instance);
    int   subdivision_count         = get_sphere_subdivision_count(g_sphere_constants);
    r.m_position                    = compute_sphere_position(radius, subdivision_count, v );

    float4x4 view                   = get_view(g_frame_constants);
    float4x4 perspective            = get_perspective(g_frame_constants);
    float4x4 world                  = translation_matrix(get_sphere_position(g_spheres, instance));

    float4 object_position          = r.m_position;

    float4 world_position           = mul(object_position, world);
    float4 view_position            = mul(world_position, view);
    float4 perspective_position     = mul(view_position, perspective);
    r.m_position                    = perspective_position;
    
    return r;
}