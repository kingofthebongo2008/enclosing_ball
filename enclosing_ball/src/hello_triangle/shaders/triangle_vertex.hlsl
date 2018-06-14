struct interpolated_value
{
	float4 m_position : SV_POSITION;
};

struct frame_constants
{
    float4x4 m_view;
    float4x4 m_perspective;
};

float4x4 get_view(frame_constants s)
{
    return s.m_view;
}

float4x4 get_perspective(frame_constants s)
{
    return s.m_perspective;
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

cbuffer g_frame_constants_buffer_cb  : register(b0) { frame_constants     g_frame_constants; };
cbuffer g_sphere_constants_buffer_cb : register(b1) { sphere_constants    g_sphere_constants; };

static const float pi = 3.141592654f;
static const float two_pi = 6.283185307f;
static const float pi_div_two = 1.570796327f;

interpolated_value main(uint v : SV_VERTEXID)
{
	interpolated_value r;
	r.m_position                    = float4(0.0f, 0.0f, 0.0f, 1.0f);

    const float radius              = get_sphere_radius(g_sphere_constants);
    const int subdivision_count     = get_sphere_subdivision_count(g_sphere_constants);

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

    float4x4 view               = get_view(g_frame_constants);
    float4x4 perspective        = get_perspective(g_frame_constants);

    float4 object_position      = float4(x, y, z, 1.0f);
    float4 view_position        = mul(object_position, view);
    float4 perspective_position = mul(view_position, perspective);

    r.m_position                = perspective_position;
    
	return r;
}