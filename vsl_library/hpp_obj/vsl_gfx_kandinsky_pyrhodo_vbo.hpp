////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_kandinsky_pyrhodo_vbo.hpp ----------
/*!
\file vsl_gfx_kandinsky_pyrhodo_vbo.hpp
\brief pyramidal rhombic dodecahedronvertex buffer methods
\author Gareth Edwards
*/

////////////////////////////////////////////////////////////////////////////////


HRESULT Gfx_Kandinsky::PyRhoDo_VBO_Config(VOID)
{

	// ---- set create tpes & vertex format
		Set(Gfx_Kandinsky_Param::COMPONENT_TYPE,
				Gfx_Kandinsky_Component::PYRHODO_VBO
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
		BOOL inside = (BOOL)Get(Gfx_Kandinsky_Param::INSIDE);
		UINT num_pyrhodo = 6;                        // six pyramidal corners...
		UINT num_prims = num_pyrhodo * 6;            // comprised of six triangles
			num_prims *= inside ? 2 : 1;             // double if inside
		UINT vertex_size = GetVertexFormatSize();
		UINT vertex_buffer_size = vertex_size * num_prims * 3;

	// ---- set buffer stuff
		Set(Gfx_Kandinsky_Param::PRIMITIVE_COUNT, num_prims);
		SetIndexBufferSize(0);
		SetVertexBufferSize(vertex_buffer_size);

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


HRESULT Gfx_Kandinsky::PyRhoDo_VBO_Create(VOID)
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
		FLOAT size = 1, move = 0;
		hr = GetParameterValue("Dimension", "Size", size);
		hr = GetParameterValue("Dimension", "Move", move);

	// ---- fx
		FLOAT explode = 0;
		hr = GetParameterValue("FX", "Explode", explode);

	// ---- shift
		FLOAT shift_x = 0 , shift_y = 0, shift_z = 0;
		hr = GetParameterValue("Shift", "X", shift_x);
		hr = GetParameterValue("Shift", "Y", shift_y);
		hr = GetParameterValue("Shift", "Z", shift_z);

	// ---- vertex components
		struct Tex2 { FLOAT u = 0, v = 0; };
		struct Vec3 { FLOAT x = 0, y = 0, z = 0; };

	// ---- pack diffuse colour
		union ColU {
			DWORD d;
			FLOAT f;
		} colu;
		colu.d = VS_D3DCOLOR_COLORVALUE(red, green, blue, alpha);
		const FLOAT df = colu.f;

	// ---- vertice
		struct Vertex_Pdnt
		{
			Vec3  position;
			Vec3  normal;
			FLOAT diffuse;
			Tex2  texture;
		};

	// ---- vertex buffer
		FLOAT *vb = pimpl_gfx_kandinsky->vertex_buffer;

	// ---- outside + inside?
		UINT num_sides = (BOOL)Get(Gfx_Kandinsky_Param::INSIDE) ? 2 : 1;
		for (UINT side = 0; side < num_sides; side++)
		{

			// ---- inside
				BOOL inside = side > 0 ? TRUE : FALSE;
				if (inside)
					explode *= -1;

			// ---- if inside flip normal
				FLOAT nf = (FLOAT)(inside ? -1 : 1);

			// ---- unit normal to 45 degree rotation is 1 / sqrt(2)
				FLOAT nx = 0.7071f * nf;
				FLOAT ny = 0.7071f * nf;
				FLOAT nz = 0.7071f * nf;

			// ---- vertex list
				const FLOAT s = size;
				const FLOAT y = s + move;
				const FLOAT Y = s * 2 + move;
				Vertex_Pdnt vl[16] =
				{
					// x   y   z   nx   ny  nz  d,  tu  tv

					{  s,  y,  s,  0,  -1,  0,  df,  0,  0    }, // 0
					{ -s,  y,  s,  0,  -1,  0,  df,  0,  1    }, // 1
					{ -s,  y, -s,  0,  -1,  0,  df,  1,  1    }, // 2
					{  s,  y, -s,  0,  -1,  0,  df,  1,  0    }, // 3

					{  s,  y,  s,  nx, ny,  0,  df,  0,    0  }, // 4
					{  0,  Y,  0,  nx, ny,  0,  df,  0.5f, 1  }, // 5
					{  s,  y, -s,  nx, ny,  0,  df,  1,    0  }, // 6

					{  s,  y, -s,   0, ny, -nz, df,  0,    0  }, // 7
					{  0,  Y,  0,   0, ny, -nz, df,  0.5f, 1  }, // 8
					{ -s,  y, -s,   0, ny, -nz, df,  1,    0  }, // 9

					{ -s,  y, -s, -nx, ny,  0,  df,  0,    0  }, // 10
					{  0,  Y,  0, -nx, ny,  0,  df,  0.5f, 1  }, // 11
					{ -s,  y,  s, -nx, ny,  0,  df,  1,    0  }, // 12

					{ -s,  y,  s,   0, ny,  nz, df,  0,    0  }, // 13
					{  0,  Y,  0,   0, ny,  nz, df,  0.5f, 1  }, // 14
					{  s,  y,  s,   0, ny,  nz, df,  1,    0  }, // 15

				};

			// ---- triangle vertex list
				Vertex_Pdnt tvl[18] =
				{
					vl[0],  vl[1],  vl[2],
					vl[0],  vl[2],  vl[3],
					vl[6],  vl[5],  vl[4],
					vl[9],  vl[8],  vl[7],
					vl[12], vl[11], vl[10],
					vl[15], vl[14], vl[13],
				};

			// ---- if inside flip vertex ordering
				if (inside)
				{
					for (UINT ivl = 0; ivl < 18; ivl += 3)
					{
						Vertex_Pdnt temp = tvl[ivl];
						tvl[ivl] = tvl[ivl+2];
						tvl[ivl+2] = temp;
					}
				}

			// ---- 6 rotation ordinate transposition sequences
				UINT rot_ots[6][3] =
				{
					{ 0, 1, 2 },
					{ 1, 0, 2 },
					{ 0, 1, 2 },
					{ 1, 0, 2 },
					{ 0, 2, 1 },
					{ 0, 2, 1 }
				};

			// ---- 6 rotation ordinate signs
				FLOAT rot_os[6][3] =
				{
					{ 1, 1, 1 },
					{ 1,-1, 1 },
					{-1,-1, 1 },
					{-1, 1, 1 },
					{ 1,-1, 1 },
					{ 1, 1,-1 }
				};

			// ---- accelerate vertex parameters
				UINT vertex_format = 0;
				hr = Get(Gfx_Kandinsky_Param::VERTEX_FORMAT, &vertex_format);


			// ---- for each vertex
				FLOAT o[3];
				UINT num_vertex = sizeof(tvl) / sizeof(Vertex_Pdnt);
				for (UINT t = 0; t < 6; t++)
				{

					for (UINT vertex = 0; vertex < num_vertex; vertex++)
					{

						Vertex_Pdnt *fvl = &tvl[vertex];
						Vec3 *p = &fvl->position;
						Vec3 *n = &fvl->normal;

						if (vertex_format & VS_XYZ)
						{
							o[0] = shift_x + p->x + n->x * explode;
							o[1] = shift_y + p->y + n->y * explode;
							o[2] = shift_z + p->z + n->z * explode;
							*(vb + 0) = o[ rot_ots[t][0] ] * rot_os[t][0];
							*(vb + 1) = o[ rot_ots[t][1] ] * rot_os[t][1];
							*(vb + 2) = o[ rot_ots[t][2] ] * rot_os[t][2];
							vb += 3;
						}

						if (vertex_format & VS_NORMAL)
						{
							o[0] = n->x;
							o[1] = n->y;
							o[2] = n->z;
							*(vb + 0) = o[ rot_ots[t][0] ] * rot_os[t][0];
							*(vb + 1) = o[ rot_ots[t][1] ] * rot_os[t][1];
							*(vb + 2) = o[ rot_ots[t][2] ] * rot_os[t][2];
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