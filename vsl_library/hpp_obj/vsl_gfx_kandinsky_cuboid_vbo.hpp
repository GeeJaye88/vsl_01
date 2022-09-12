////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_kandinsky_cuboid_vbo.hpp ----------
/*!
\file vsl_gfx_kandinsky.cpp
\file vsl_gfx_kandinsky_cuboid_vbo.hpp
\author Gareth Edwards
*/


////////////////////////////////////////////////////////////////////////////////


HRESULT Gfx_Kandinsky::Cuboid_VBO_Config(VOID)
{

	// ---- set create types & vertex format
		Set(Gfx_Kandinsky_Param::COMPONENT_TYPE,
				Gfx_Kandinsky_Component::COMPONENT_UNKNOWN
			);

		Set(Gfx_Kandinsky_Param::PRIMITIVE_TYPE,
				Gfx_Kandinsky_Primitive_Type::TRIANGLELIST
			);

		Set(Gfx_Kandinsky_Param::PRIMITIVE_COUNT,
				12 // 6 faces split into 2 triangles
			);

		Set(Gfx_Kandinsky_Param::VERTEX_FORMAT,
				Gfx_Kandinsky_Vertex_Format::XYZ |
				Gfx_Kandinsky_Vertex_Format::NORMAL |
				Gfx_Kandinsky_Vertex_Format::DIFFUSE |
				Gfx_Kandinsky_Vertex_Format::TEX1
			);

	// ---- build
		SetIndexBufferSize(0);

	// ---- set buffer stuff
		UINT vertex_buffer_size = GetVertexFormatSize() *
			Get(Gfx_Kandinsky_Param::PRIMITIVE_COUNT) * 3;
		SetVertexBufferSize(vertex_buffer_size);

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


HRESULT Gfx_Kandinsky::Cuboid_VBO_Create(VOID)
{

	// ---- parameters
		HRESULT hr;
		FLOAT red = 1, green = 1, blue = 1, alpha = 1;
		
	// ---- colour
		hr = GetParameterValue("Colour", "Red",   red);
		hr = GetParameterValue("Colour", "Green", green);
		hr = GetParameterValue("Colour", "Blue",  blue);
		hr = GetParameterValue("Colour", "Alpha", alpha);

	// ---- dimensions
		FLOAT height = 1, width = 1, depth = 1;
		hr = GetParameterValue("Dimension", "Height", height);
		hr = GetParameterValue("Dimension", "Width",  width);
		hr = GetParameterValue("Dimension", "Depth",  depth);

	// ---- fx
		FLOAT explode = 0;
		hr = GetParameterValue("FX", "Explode", explode);

	// ---- shift
		FLOAT shift_x = 0 , shift_y = 0, shift_z = 0;
		hr = GetParameterValue("Shift", "X", shift_x);
		hr = GetParameterValue("Shift", "Y", shift_y);
		hr = GetParameterValue("Shift", "Z", shift_z);

	// ---- centred dimensions
		const FLOAT wby2 = width / 2;
		const FLOAT hby2 = height / 2;
		const FLOAT dby2 = depth / 2;

	// ---- vertex components
		struct Tex2 { FLOAT u = 0, v = 0; };
		struct Vec3 { FLOAT x = 0, y = 0, z = 0; };

	// pack diffuse colour
		union ColU {
			DWORD d;
			FLOAT f;
		} colu;
		colu.d = VS_D3DCOLOR_COLORVALUE(red,green,blue,alpha);
		const FLOAT diffuse = colu.f;

	// ---- vertices
		struct Vertex_Pdnt
		{
			Vec3  position;
			FLOAT diffuse;
			Vec3  normal;
			Tex2  texture;
		};

	// ---- faces
		const FLOAT wp = wby2, wn = -wby2;
		const FLOAT hp = hby2, hn = -hby2;
		const FLOAT dp = dby2, dn = -dby2;
		const FLOAT df = diffuse;
		Vertex_Pdnt face_vertex_list[6][4] =
		{
			{ // 0
				{  wp, hp, dp, df, 0, 0, 1, 0, 0 },
				{  wn, hp, dp, df, 0, 0, 1, 1, 0 },
				{  wn, hn, dp, df, 0, 0, 1, 1, 1 },
				{  wp, hn, dp, df, 0, 0, 1, 0, 1 }
			},
			{ // 1 
				{ wp, hp, dn, df, 1, 0, 0, 0, 0 },
				{ wp, hp, dp, df, 1, 0, 0, 1, 0 },
				{ wp, hn, dp, df, 1, 0, 0, 1, 1 },
				{ wp, hn, dn, df, 1, 0, 0, 0, 1 }
			},
			{ // 2
				{ wn, hp, dn, df, 0, 0,-1, 0, 0 },
				{ wp, hp, dn, df, 0, 0,-1, 1, 0 },
				{ wp, hn, dn, df, 0, 0,-1, 1, 1 },
				{ wn, hn, dn, df, 0, 0,-1, 0, 1 }
			},
			{ // 3
				{ wn, hp, dp, df,-1, 0, 0, 0, 0 },
				{ wn, hp, dn, df,-1, 0, 0, 1, 0 },
				{ wn, hn, dn, df,-1, 0, 0, 1, 1 },
				{ wn, hn, dp, df,-1, 0, 0, 0, 1 }
			},
			{ // 4
				{ wn, hp, dp, df, 0, 1, 0, 0, 0 },
				{ wp, hp, dp, df, 0, 1, 0, 1, 0 },
				{ wp, hp, dn, df, 0, 1, 0, 1, 1 },
				{ wn, hp, dn, df, 0, 1, 0, 0, 1 }
			},
			{ // 5
				{ wp, hn, dp, df, 0,-1, 0, 0, 0 },
				{ wn, hn, dp, df, 0,-1, 0, 1, 0 },
				{ wn, hn, dn, df, 0,-1, 0, 1, 1 },
				{ wp, hn, dn, df, 0,-1, 0, 0, 1 }
			}
		};

	// ---- calculate # of faces
		const UINT num_face = sizeof(face_vertex_list) / (sizeof(Vertex_Pdnt) *4);
	
	// ---- calculate # of primitives
		const UINT num_primitives = num_face * 2;

	// ---- accelerate vertex parameters
		UINT vertex_format = 0;
		hr = Get(Gfx_Kandinsky_Param::VERTEX_FORMAT, &vertex_format);

	// ---- triangle vertex index table
		const UINT triangle_vertex_index[2][3]
		{
			{0, 1, 2},
			{0, 2, 3}
		};

	// ---- vertex buffer
		FLOAT *vb = pimpl_gfx_kandinsky->vertex_buffer;
		FLOAT *vb_start = vb;

	// ---- for each face
		for (UINT face = 0; face < num_face; face++)
		{

			// --- for each triangle
				for (UINT triangle = 0; triangle < 2; triangle++)
				{

					// ---- for each triangle vertex
						for (UINT vertex = 0; vertex < 3; vertex++)
						{

								Vertex_Pdnt *fvl = &face_vertex_list[face][ triangle_vertex_index[triangle][vertex] ];
								Vec3 *p = &fvl->position;
								Vec3 *n = &fvl->normal;

								if (vertex_format & VS_XYZ)
								{
									*(vb + 0) = shift_x + p->x + n->x * explode;
									*(vb + 1) = shift_y + p->y + n->y * explode;
									*(vb + 2) = shift_z + p->z + n->z * explode;
									vb += 3;
								}

								if (vertex_format & VS_NORMAL)
								{
									*(vb + 0) = n->x;
									*(vb + 1) = n->y;
									*(vb + 2) = n->z;
									vb += 3;
								}

								if (vertex_format & VS_DIFFUSE)
								{
									*(vb) = fvl->diffuse; vb++;
								}

								if (vertex_format & VS_SPECULAR)
								{
									*(vb) = 1;
									vb++;
								}

								if (vertex_format & VS_TEX1)
								{
									*(vb + 0) = fvl->texture.u;
									*(vb + 1) = fvl->texture.v;
									vb += 2;
								}
						}

				}
		}

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


HRESULT Gfx_Kandinsky::Cuboid_v1(VOID)
{

	// ---- centred dimensions
		FLOAT wby2 = 5;
		FLOAT hby2 = 5;
		FLOAT dby2 = 5;


	// ---- vertex components
		struct Tex2 { FLOAT u = 0, v = 0; };
		struct Vec3 { FLOAT x = 0, y = 0, z = 0; };

		// pack diffuse colour
		union ColU {
			DWORD d;
			FLOAT f;
		} colu;
		colu.d = VS_D3DCOLOR_COLORVALUE(1, 1, 1, 1);
		FLOAT diffuse = colu.f;


	// ---- quad vertices
		struct quad_vertex
		{
			Vec3  position;
			FLOAT diffuse;
			Vec3  normal;
			Tex2  texture;
		};

		quad_vertex shapes_list[12][4] =
		{
			{ // 0
				{   wby2,  hby2,  0,  diffuse,  0,  0,  1,  0,  0 },
				{  -wby2,  hby2,  0,  diffuse,  0,  0,  1,  1,  0 },
				{  -wby2, -hby2,  0,  diffuse,  0,  0,  1,  1,  1 },
				{   wby2, -hby2,  0,  diffuse,  0,  0,  1,  0,  1 }
			},
			{ // 1 
				{   0,  hby2, -dby2,  diffuse,  1,  0,  0,  0,  0 },
				{   0,  hby2,  dby2,  diffuse,  1,  0,  0,  1,  0 },
				{   0, -hby2,  dby2,  diffuse,  1,  0,  0,  1,  1 },
				{   0, -hby2, -dby2,  diffuse,  1,  0,  0,  0,  1 }
			},
			{ // 2
				{  -wby2,  hby2,  0,  diffuse,  0,  0, -1,  0,  0 },
				{   wby2,  hby2,  0,  diffuse,  0,  0, -1,  1,  0 },
				{   wby2, -hby2,  0,  diffuse,  0,  0, -1,  1,  1 },
				{  -wby2, -hby2,  0,  diffuse,  0,  0, -1,  0,  1 }
			},
			{ // 3
				{   0,  hby2,  dby2,  diffuse, -1,  0,  0,  0,  0 },
				{   0,  hby2, -dby2,  diffuse, -1,  0,  0,  1,  0 },
				{   0, -hby2, -dby2,  diffuse, -1,  0,  0,  1,  1 },
				{   0, -hby2,  dby2,  diffuse, -1,  0,  0,  0,  1 }
			},
			{ // 4
				{  -wby2,  0,  dby2,  diffuse,  0,  1,  0,  0,  0 },
				{   wby2,  0,  dby2,  diffuse,  0,  1,  0,  1,  0 },
				{   wby2,  0, -dby2,  diffuse,  0,  1,  0,  1,  1 },
				{  -wby2,  0, -dby2,  diffuse,  0,  1,  0,  0,  1 }
			},
			{ // 5
				{   wby2,  0,  dby2,  diffuse,  0, -1,  0,  0,  0 },
				{  -wby2,  0,  dby2,  diffuse,  0, -1,  0,  1,  0 },
				{  -wby2,  0, -dby2,  diffuse,  0, -1,  0,  1,  1 },
				{   wby2,  0, -dby2,  diffuse,  0, -1,  0,  0,  1 }
			},
			{ // 6
				{  -wby2,  hby2,  0,  diffuse,  0,  0, -1,  0,  0 },
				{   wby2,  hby2,  0,  diffuse,  0,  0, -1,  1,  0 },
				{   wby2, -hby2,  0,  diffuse,  0,  0, -1,  1,  1 },
				{  -wby2, -hby2,  0,  diffuse,  0,  0, -1,  0,  1 }
			},
			{ // 7
				{   0,  hby2,  dby2,  diffuse, -1,  0,  0,  0,  0 },
				{   0,  hby2, -dby2,  diffuse, -1,  0,  0,  1,  0 },
				{   0, -hby2, -dby2,  diffuse, -1,  0,  0,  1,  1 },
				{   0, -hby2,  dby2,  diffuse, -1,  0,  0,  0,  1 }
			},
			{ // 8
				{   wby2,  hby2,  0,  diffuse,  0,  0,  1,  0,  0 },
				{  -wby2,  hby2,  0,  diffuse,  0,  0,  1,  1,  0 },
				{  -wby2, -hby2,  0,  diffuse,  0,  0,  1,  1,  1 },
				{   wby2, -hby2,  0,  diffuse,  0,  0,  1,  0,  1 }
			},
			{ // 9
				{   0,  hby2, -dby2,  diffuse,  1,  0,  0,  0,  0 },
				{   0,  hby2,  dby2,  diffuse,  1,  0,  0,  1,  0 },
				{   0, -hby2,  dby2,  diffuse,  1,  0,  0,  1,  1 },
				{   0, -hby2, -dby2,  diffuse,  1,  0,  0,  0,  1 }
			},
			{ // 10
				{   wby2,  0,  dby2,  diffuse,  0, -1,  0,  0,  0 },
				{  -wby2,  0,  dby2,  diffuse,  0, -1,  0,  1,  0 },
				{  -wby2,  0, -dby2,  diffuse,  0, -1,  0,  1,  1 },
				{   wby2,  0, -dby2,  diffuse,  0, -1,  0,  0,  1 }
			},
			{ // 11
				{  -wby2,  0,  dby2,  diffuse,  0,  1,  0,  0,  0 },
				{   wby2,  0,  dby2,  diffuse,  0,  1,  0,  1,  0 },
				{   wby2,  0, -dby2,  diffuse,  0,  1,  0,  1,  1 },
				{  -wby2,  0, -dby2,  diffuse,  0,  1,  0,  0,  1 }

			}
		};


	// ---- init stuff
		UINT num_quad = sizeof(shapes_list) / (sizeof(quad_vertex) *4);

		FLOAT explode = 0;
		FLOAT shift_x = 0;
		FLOAT shift_y = 0;
		FLOAT shift_z = 0;

		FLOAT *vb     = pimpl_gfx_kandinsky->vertex_buffer;


	// ---- triangle vertex index table
		UINT triangle_vertex_index[2][3]
		{
			{0, 1, 2},
			{0, 2, 3}
		};


	// ---- config parameters
		UINT num_primitives = num_quad * 2;
		UINT vertex_format = 0;
		HRESULT hr = Get(Gfx_Kandinsky_Param::VERTEX_FORMAT, &vertex_format);


	// ---- for each shape
		for (UINT shape = 0; shape< num_quad; shape++)
		{

			// --- for each triangle
				for (UINT triangle = 0; triangle < 2; triangle++)
				{

					// ---- for each triangle vertex
						for (UINT vertex = 0; vertex < 3; vertex++)
						{

								quad_vertex *sv = &shapes_list[shape][ triangle_vertex_index[triangle][vertex] ];
								Vec3 *p = &sv->position;
								Vec3 *n = &sv->normal;

								if (vertex_format & VS_XYZ)
								{
									*(vb + 0) = shift_x + p->x + n->x * explode;
									*(vb + 1) = shift_y + p->y + n->y * explode;
									*(vb + 2) = shift_z + p->z + n->z * explode;
									//*(vb + 0) = shift_x + p->x + n->x * (explode + p->x) * wby2;
									//*(vb + 1) = shift_y + p->y + n->y * (explode + p->y) * hby2;
									//*(vb + 2) = shift_z + p->z + n->z * (explode + p->z) * dby2;
									vb += 3;
								}

								if (vertex_format & VS_NORMAL)
								{
									*(vb + 0) = n->x;
									*(vb + 1) = n->y;
									*(vb + 2) = n->z;
									vb += 3;
								}

								if (vertex_format & VS_DIFFUSE)
								{
									*(vb) = sv->diffuse; vb++;
								}

								if (vertex_format & VS_SPECULAR)
								{
									*(vb) = 1;
									vb++;
								}

								if (vertex_format & VS_TEX1)
								{
									*(vb + 0) = sv->texture.u;
									*(vb + 1) = sv->texture.v;
									vb += 2;
								}
						}

				}
		}

	return 0;
}


////////////////////////////////////////////////////////////////////////////////