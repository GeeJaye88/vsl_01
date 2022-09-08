////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_mesh3d.h ----------
/*!
\file vsl_mesh3d.h
\brief interface for the Mesh3D class
\author Gareth Edwards
*/


////////////////////////////////////////////////////////////////////////////////


// ---- system include
	#include "../../vsl_system/header/vsl_include.h"
	#include "../../vsl_system/header/vsl_win_structs.h"

// ---- application include
	#include "../../vsl_application/shared/header/vsl_fvf_vertex_structs.h"


////////////////////////////////////////////////////////////////////////////////


// ---------- Mesh3D class interface ----------

namespace vsl_application
{

	class Mesh3D  
	{

	public:

		// ---- cdtor
			Mesh3D ();
			virtual ~Mesh3D ();


		 // ---- system methods
			HRESULT Fw_Setup();
			HRESULT Fw_SetupDX(LPDIRECT3DDEVICE9);
			HRESULT Fw_Display(LPDIRECT3DDEVICE9);
			HRESULT Fw_CleanupDX(LPDIRECT3DDEVICE9);
			HRESULT Fw_Cleanup();

		// ---- system struct methods
			VOID Fw_Get_Win_Create(vsl_system::Win_Create **wc);
			VOID Fw_Get_Win_Engine(vsl_system::Win_Engine **wc);

		// ---- system notify event methods
			VOID Fw_Set_MouseLeftButtonDownMove(INT, INT);
			VOID Fw_Set_MouseWheelClick(INT);
			VOID Fw_Set_Keydown(WPARAM param);


		private:

		// ---- system structs
			vsl_system::Win_Create fw_win_create;
			vsl_system::Win_Engine fw_win_engine;


		public:

		// ---- application methods
		
		
		// ---- cube
			VOID CubeSideSetup(LPDIRECT3DDEVICE9, FLOAT, FLOAT);
			VOID CubePyramidSideSetup(LPDIRECT3DDEVICE9, FLOAT, FLOAT);
			VOID CubeDisplay(LPDIRECT3DDEVICE9, D3DXMATRIX, FLOAT);


		// ---- teapot
			VOID TeapotsSetup(LPDIRECT3DDEVICE9);
			VOID TeapotsDisplay(LPDIRECT3DDEVICE9, D3DXMATRIX, FLOAT, INT);


		// ---- mesh - a sine wave
			struct mesh_sine_wave
			{
				FLOAT x_emitter   = 0;
				FLOAT z_emitter   = 0;
				FLOAT amplitude   = 1;
				FLOAT period      = 90;
				FLOAT phase_shift = 0;
				BOOL  status    = true;
			};


		// ---- mesh framework
			VOID MeshSetup(LPDIRECT3DDEVICE9, FLOAT*);
			VOID MeshInitialise(LPDIRECT3DDEVICE9);
			VOID MeshDisplay(LPDIRECT3DDEVICE9, D3DXMATRIX *, FLOAT);

		// ---- mesh normals
			VOID MeshCalculateGoodEnoughNormals();
			VOID MeshCalculateCorrectNormals();
			VOID MeshCalculateDisplayedNormals();

		// ---- mesh variants
			VOID MeshSineWaveOriginal(FLOAT);
			VOID MeshSineWaveMultiple(FLOAT);
			VOID MeshSineWaveMultipleNew(FLOAT);
			VOID MeshSineWaveMultipleNewCalc(mesh_sine_wave *);

		// ---- multiple Gerstner 3D displacement waves

			// wind direction
			struct GerstnerVec2
			{
				FLOAT x;
				FLOAT y;
				GerstnerVec2::GerstnerVec2(FLOAT _x, FLOAT _y) { x = _x; y = _y; }
			};

			// surface displacement
			struct GerstnerVec3
			{
				FLOAT x;
				FLOAT y;
				FLOAT z;
				GerstnerVec3::GerstnerVec3(FLOAT _x, FLOAT _y, FLOAT _z) { x = _x; y = _y; z = _z; }
			};

			// dot product - I only bothered with this to demonstrate maths
			static double dot(GerstnerVec2 v, GerstnerVec2 u) {
				return v.x * u.x + v.y * u.y;
			}

			// single wave direction and parameters
			struct GerstnerWave
			{
				GerstnerVec2 dir;
				FLOAT amplitude;
				FLOAT wave_length;
			};

			// multiple wave parameters
			struct GerstnerWaves
			{
				UINT num_waves;
				FLOAT wave_steepness;
				FLOAT wave_speed;
			};

			// update multiple Gerstner waves
			VOID MeshGerstner(FLOAT);

			// update single Gerstner wave vertex
			static GerstnerVec3 CalcGerstnerWaveOffset(
					GerstnerWaves &waves,
					GerstnerWave  wave_list[],
					GerstnerVec3 location,
					FLOAT time
				);

		// ---- application display methods
			VOID  Display_Text();
			VOID  Display_SetDefaults();
			VOID  Display_AsyncKeyState();


	private:

		// ---- mesh properties


		// ---- gui
			FLOAT  x_rotate = 0;
			FLOAT  y_rotate = 0;
			FLOAT  z_translate = 0;
			DWORD  key_just_pressed = 0;
			DWORD  key_last_pressed = 0;
			DOUBLE y_turntable = 0;
			BOOL   runtime = TRUE;

		// ---- cube
			LPDIRECT3DVERTEXBUFFER9	p_cube_side_vertex_buffer;
			D3DMATERIAL9			cube_side_material;
			INT                     cube_side_num_primitives;
			FLOAT                   cube_speed = 0.25f;

		// ---- teapot
			LPD3DXMESH				p_teapot_mesh[5];
			D3DMATERIAL9			teapot_material;
			FLOAT                   teapot_speed = 0.1f;

		// ---- mesh

			FLOAT        mesh_param[8];

			DWORD        mesh_vertex_format;  //!< FVF - flexible vertex format
			DWORD        mesh_vertex_size;    //!< size of vertex format (e.g. sizeof(vertex3x))
			DWORD        mesh_vertices;       //!< number of vertices
			DWORD        mesh_faces;          //!< number of faces
			DWORD        mesh_vertex_columns; //!< number of vertex columns
			DWORD        mesh_vertex_rows;    //!< number of vertex rows
			DWORD        mesh_cel_columns;    //!< number of cell columns
			DWORD        mesh_cel_rows;       //!< number of cell rows
			LPD3DXMESH   p_mesh;         //!< the d3d mesh object
			LPDIRECT3DTEXTURE9 p_mesh_texture;
			D3DMATERIAL9       mesh_material;


		// ---- calculation buffer
			INT          mesh_num_calc_vertices;
			Vertex      *mesh_calc_vertices;
			Vertex      *mesh_calc_normals;
			Vertex      *mesh_calc_points;

		// ---- display normals
			Vertex      *mesh_normal_vertices;
			INT          mesh_num_normals;
			D3DMATERIAL9 mesh_normal_material;


		// --- display

			enum
			{
				CUBE_OBJECT,
				TEAPOT_OBJECT,
				MESH_OBJECT,
			};
			BYTE object_displayed = MESH_OBJECT;

			enum
			{
				SINGLE_SINE,
				MULTIPLE_SINE,
				PARAMETRIC_SINE,
				GERSTNER_WAVE
			};
			UINT mesh_type = PARAMETRIC_SINE;

			BOOL mesh_display_normals = FALSE;
			BOOL mesh_display_solid   = TRUE;


		// ---- scene
			D3DLIGHT9 sun_light;
			D3DLIGHT9 back_light;


		// ---- not used - but try putting this in as the time parameter:
			//  
			//  CalcGerstnerWaveOffset(waves, wave_list, position, gerstner_time);
			//
			//	then incrementing - gerstner_time += 0.1f; - outside the vertexc loop
			//
			DOUBLE gerstner_time = 0;


		// ---- display text font
			LPD3DXFONT font;

	};

}


////////////////////////////////////////////////////////////////////////////////



