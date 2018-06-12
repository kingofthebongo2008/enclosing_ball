struct interpolated_value
{
	float4 m_position : SV_POSITION;
};

static const float radius             = 0.2f;
static const int subdivision_count    = 20;

static const float pi = 3.141592654f;
static const float two_pi = 6.283185307f;
static const float pi_div_two = 1.570796327f;


interpolated_value main(uint v : SV_VERTEXID)
{
	interpolated_value r;
	r.m_position                    = float4(0.0f, 0.0f, 0.0f, 1.0f);

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

    r.m_position.x = x;
    r.m_position.y = y;
    r.m_position.z = z;
    r.m_position.w = 1.0f;

	return r;
}