#include "triangle_constants.h"
#include "draw_sphere.hlsli"

struct interpolated_value
{
    float4 m_position : SV_POSITION;
};

cbuffer g_frame_constants_buffer_cb     : register(b0) { frame_constants     g_frame_constants; };
cbuffer g_sphere_constants_buffer_cb    : register(b1) { sphere_constants    g_sphere_constants; };
cbuffer g_object_constants_buffer_cb    : register(b2) { object_constants    g_object_constants; };

interpolated_value main(uint v : SV_VERTEXID)
{
    interpolated_value r;

    float radius                    = get_sphere_radius(g_sphere_constants);
    int   subdivision_count         = get_sphere_subdivision_count(g_sphere_constants);

    r.m_position                    = compute_sphere_position( radius, subdivision_count, v );

    float4x4 view                   = get_view(g_frame_constants);
    float4x4 perspective            = get_perspective(g_frame_constants);
    float4x4 world                  = get_world(g_object_constants);

    float4 object_position          = r.m_position;

    float4 world_position           = mul(object_position, world);
    float4 view_position            = mul(world_position, view);
    float4 perspective_position     = mul(view_position, perspective);
    r.m_position                    = perspective_position;
    
    return r;
}