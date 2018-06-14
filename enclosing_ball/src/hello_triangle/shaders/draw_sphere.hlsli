#ifndef __draw_sphere_hlsli__
#define __draw_sphere_hlsli__

#include "triangle_constants.h"


float4 compute_sphere_position(sphere_constants sphere, uint v)
{
    const float radius              = get_sphere_radius(sphere);
    const int subdivision_count     = get_sphere_subdivision_count(sphere);

    const int vertical_segments     = subdivision_count;
    const int horizontal_segments   = subdivision_count * 2;
    const int vertex_count          = (vertical_segments + 1) * (horizontal_segments + 1);
    const int index_count           = vertex_count * 3;

    const int i                     = v % (vertical_segments + 1);
    const int j                     = v / horizontal_segments;

    float lattitude                 = (((float)i / vertical_segments) * pi) - pi_div_two;
    float longitude                 = ( (float)j * two_pi / horizontal_segments) - pi;

    float dxz;
    float dx;
    float dz;
    float dy;

    dy  = sin(lattitude);
    dx  = sin(longitude);
    dz  = cos(longitude);
    dxz = cos(lattitude);

    dx  = dx * dxz;
    dz  = dz * dxz;

    float x, y, z;

    x   = dx * radius;
    y   = dy * radius;
    z   = dz * radius;

    return float4(x, y, z, 1.0f);
}

#endif