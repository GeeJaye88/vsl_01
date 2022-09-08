////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_kandinsky_cuboid_vibo.hpp ----------
/*!
\file vsl_gfx_kandinsky_cuboid_vibo.cpp
\brief cuboid vertex & index buffer methods
\author Gareth Edwards 
*/

////////////////////////////////////////////////////////////////////////////////


HRESULT Gfx_Kandinsky::Cuboid_VIBO_Config()
{

	// ---- set create tpes & vertex format
		Set(Gfx_Kandinsky_Param::COMPONENT_TYPE,
				Gfx_Kandinsky_Component::COMPONENT_UNKNOWN
			);

		Set(Gfx_Kandinsky_Param::PRIMITIVE_TYPE,
				Gfx_Kandinsky_Primitive_Type::TRIANGLELIST
			);

		Set(Gfx_Kandinsky_Param::VERTEX_FORMAT,
				Gfx_Kandinsky_Vertex_Format::XYZ |
				Gfx_Kandinsky_Vertex_Format::NORMAL |
				Gfx_Kandinsky_Vertex_Format::DIFFUSE |
				Gfx_Kandinsky_Vertex_Format::TEX1
			);

	// ---- build
		UINT num_faces = 6;
		UINT num_prims = num_faces * 2;
		UINT index_buffer_size = num_prims * 3;
		UINT vertex_size = GetVertexFormatSize();
		UINT vertex_buffer_size = vertex_size * num_prims * 3;

	// ---- set buffer stuff
		Set(Gfx_Kandinsky_Param::PRIMITIVE_COUNT, num_prims);
		SetIndexBufferSize(index_buffer_size);
		SetVertexBufferSize(vertex_buffer_size);

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


HRESULT Gfx_Kandinsky::Cuboid_VIBO_Create()
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

	// ---- pack diffuse colour
		union ColU {
			DWORD d;
			FLOAT f;
		} colu;
		colu.d = VS_D3DCOLOR_COLORVALUE(red, green, blue, alpha);
		const FLOAT diffuse = colu.f;

	// ---- vertices
		struct Vertex_Pdnt
		{
			Vec3  position;
			Vec3  normal;
			FLOAT diffuse;
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
				{ wp, hp, dp, 0, 0, 1, df, 0, 0 },
				{ wn, hp, dp, 0, 0, 1, df, 1, 0 },
				{ wn, hn, dp, 0, 0, 1, df, 1, 1 },
				{ wp, hn, dp, 0, 0, 1, df, 0, 1 }
			},
			{ // 1 
				{ wp, hp, dn, 1, 0, 0, df, 0, 0 },
				{ wp, hp, dp, 1, 0, 0, df, 1, 0 },
				{ wp, hn, dp, 1, 0, 0, df, 1, 1 },
				{ wp, hn, dn, 1, 0, 0, df, 0, 1 }
			},
			{ // 2
				{ wn, hp, dn, 0, 0,-1, df, 0, 0 },
				{ wp, hp, dn, 0, 0,-1, df, 1, 0 },
				{ wp, hn, dn, 0, 0,-1, df, 1, 1 },
				{ wn, hn, dn, 0, 0,-1, df, 0, 1 }
			},
			{ // 3
				{ wn, hp, dp,-1, 0, 0, df, 0, 0 },
				{ wn, hp, dn,-1, 0, 0, df, 1, 0 },
				{ wn, hn, dn,-1, 0, 0, df, 1, 1 },
				{ wn, hn, dp,-1, 0, 0, df, 0, 1 }
			},
			{ // 4
				{ wn, hp, dp, 0, 1, 0, df, 0, 0 },
				{ wp, hp, dp, 0, 1, 0, df, 1, 0 },
				{ wp, hp, dn, 0, 1, 0, df, 1, 1 },
				{ wn, hp, dn, 0, 1, 0, df, 0, 1 }
			},
			{ // 5
				{ wp, hn, dp, 0,-1, 0, df, 0, 0 },
				{ wn, hn, dp, 0,-1, 0, df, 1, 0 },
				{ wn, hn, dn, 0,-1, 0, df, 1, 1 },
				{ wp, hn, dn, 0,-1, 0, df, 0, 1 }
			}
		};

	// ---- calculate # of faces
		const UINT num_face = sizeof(face_vertex_list) / (sizeof(Vertex_Pdnt) *4);

	// ---- accelerate vertex parameters
		UINT vertex_format = 0;
		hr = Get(Gfx_Kandinsky_Param::VERTEX_FORMAT, &vertex_format);

	// ---- vertex buffer
		FLOAT *vb = pimpl_gfx_kandinsky->vertex_buffer;

	// ---- for each face
		for (UINT face = 0; face < num_face; face++)
		{

			// ---- for each vertex
				for (UINT vertex = 0; vertex < 4; vertex++)
				{

						Vertex_Pdnt *fvl = &face_vertex_list[face][vertex];
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

	// ---- index buffer
		UINT *ib = pimpl_gfx_kandinsky->index_buffer;

	// ---- triangle vertex index table
		const UINT triangle_vertex_index[2][3]
		{
			{0, 1, 2},
			{0, 2, 3}
		};

	// ---- for each face
		for (UINT face = 0; face < num_face; face++)
		{
			const UINT vertex_offset = face * 4;
			for (UINT row = 0; row < 2; row++)
			{
				for (UINT col = 0; col < 3; col++)
				{
					*(ib++) = vertex_offset + triangle_vertex_index[row][col];
				}
			}
		}

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////