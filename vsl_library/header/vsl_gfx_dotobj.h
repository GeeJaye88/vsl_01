////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_dotobj.h ----------
/*!
\file vsl_dotobj.h
\brief Interface for the DotObjUtilities class.
\author Gareth Edwards
*/

#pragma once

// ---- vs system
	#include "../../vsl_system/header/vsl_include.h"


////////////////////////////////////////////////////////////////////////////////


// ---------- namespace ----------

namespace vs_library
{

	class DotObjElement
	{

		public:

			enum TagId
			{
				COMMENT          = 1,
				VERTEX_GEOMETRY  = 2,
				VERTEX_TEXTURE   = 3,
				VERTEX_NORMAL    = 4,
				VERTEX_PARAMETER = 5,
				ELEMENT_FACE     = 6,
				ELEMENT_LINE     = 7,
				ELEMENT_POINT    = 8,
				UNDEFINED        = 9
			};

		// ---- cdtor
			DotObjElement::DotObjElement();
			DotObjElement::~DotObjElement();

		// ---- builder
			DotObjElement *Add(TagId tid);

		// ---- node
			DotObjElement *GetFirst();
			DotObjElement *GetLast();
			DotObjElement *GetNext();
			DotObjElement *GetParent();
			DotObjElement *GetPrevious();
			TagId          GetTagId();

		// ---- data 
			INT            GetNumberOfValues();
			INT            SetNumberOfValues(INT);
			INT            GetIntValue(INT index);
			INT            SetIntValue(INT index, INT value);
			FLOAT          GetFloatValue(INT index);
			INT            SetFloatValue(INT index, FLOAT value);
			FLOAT*         GetPtrToFloatValues();

	private:

		// ---- private implementation
			class  PI_DotObjElement; PI_DotObjElement *pi_dot_obj_element;

	};


	class DotObjUtilities
	{

		public:

			enum GfxRender
			{
				RAW        = 1,
				PROCESSED  = 2,
			};

		// ---- cdtor
			DotObjUtilities::DotObjUtilities();
			DotObjUtilities::~DotObjUtilities();

		// ---- io framework
			HRESULT Read(DotObjElement *object, CHAR *filename);
			HRESULT Report(DotObjElement *object, CHAR *filename);
			HRESULT Write(DotObjElement *object, CHAR *filename);

		// ---- get size of list
			INT GetSizeOfIndexList(DotObjElement *object, DotObjElement::TagId tag_id);
			INT GetSizeOfVertexList(DotObjElement *object, DotObjElement::TagId tag_id);

		// ---- get buffers
			INT GetIndexBuffer(DotObjElement *object, DotObjElement::TagId tag_id, DWORD *index_buffer, INT index_count);
			INT GetVertexBuffer(DotObjElement *object, DotObjElement::TagId tag_id, FLOAT *vertex_buffer, INT vertex_count, INT vertex_format);

		// ---- get triangle stuff
			INT GetNumberOfTriangles(DotObjElement *object);
			INT GetTriangleIndexBuffer(DotObjElement *object, DWORD *index_buffer, INT index_count);
			INT GetTriangleVertexBuffer(DotObjElement *object, FLOAT *object_param, FLOAT *vertex_buffer, INT vertex_total, INT vertex_count);

		// ---- methods to be made private
			HRESULT CreateTagLists(DotObjElement *object);
			HRESULT ParseData(DotObjElement *object, CHAR *data , INT size);
			HRESULT ParseLine(DotObjElement *object, CHAR *data , INT len);
		
		private:

		// ---- private implementation
			class  PI_DotObjUtilities; PI_DotObjUtilities *pi_dot_obj_utilities;

	};

};
