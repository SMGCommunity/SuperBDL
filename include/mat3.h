#ifndef __MAT3_H
#define __MAT3_H

#include <stdbool.h>
#include <bmd.h>
#include <vector.h>
#include <matrix.h>

#define MAGIC_MAT3 "MAT3"

bool writeMAT3(model_data data);


struct Material
{
	char* name;


};

struct LightChannel
{
	bool enable;
	ColorSrc colorSource;
	LightId selectedLights;
	DiffuseFunction diffuseFunction;
	AttenuationFunction attenuationFunction;
	ColorSrc ambientSource;
};

struct TexCoordGen
{
	TexGenType type;
	TexGenSrc source;
	TexGenMatrix matrixID;
};

struct TexMatrix
{
	TexGenType type;
	TexMtxMapMode mode;
	/// <summary>
	/// Use Maya calculation or not. (default false)
	/// </summary>
	bool useMaya;

	Vector3f center;
	Vector2f scale;
	float rotation;
	Vector2f translation;
	Matrix4x4f effectMatrix;
};

enum ColorSrc
{
	/// <summary>
	/// Use Register Colors
	/// </summary>
	REGISTER = 0,
	/// <summary>
	/// Use Vertex Colors
	/// </summary>
	VERTEX = 1
};

enum LightId
{
	NONE = 0x000,
	LIGHT0 = 0x001,
	LIGHT1 = 0x002,
	/// <summary>
	/// Commonly used by SMG for lighting effects
	/// </summary>
	LIGHT2 = 0x004,
	LIGHT3 = 0x008,
	LIGHT4 = 0x010,
	LIGHT5 = 0x020,
	LIGHT6 = 0x040,
	LIGHT7 = 0x080
};

enum DiffuseFunction {
	NONE = 0x00,
	SIGN = 0x01,
	CLAMP = 0x02,
};
enum AttenuationFunction {
	SPECULAR = 0x00, // Specular attenuation
	SPOTLIGHT = 0x01, // Distance/spotlight attenuation
	NONE = 0x02,
};

enum GXRenderMode
{
	/// <summary>
	/// Opaque
	/// </summary>
	OPAQUE = 0x01,
	/// <summary>
	/// Stencil Alpha (?)
	/// </summary>
	EDGE = 0x02,
	/// <summary>
	/// Full Alpha
	/// </summary>
	TRANSLUCENT = 0x04
};

enum GXCullMode
{
	/// <summary>
	/// Do not cull either side
	/// </summary>
	NONE = 0,
	/// <summary>
	/// Cull front-facing primitives
	/// </summary>
	FRONT = 1,
	/// <summary>
	/// Cull back-facing primitives
	/// </summary>
	BACK = 2,
	/// <summary>
	/// Cull both sides
	/// </summary>
	ALL = 3
};

enum TexGenType {
	MATRIX3x4 = 0,
	/// <summary>
	/// The most common
	/// </summary>
	MATRIX2x4 = 1,
	BUMP0 = 2,
	BUMP1 = 3,
	BUMP2 = 4,
	BUMP3 = 5,
	BUMP4 = 6,
	BUMP5 = 7,
	BUMP6 = 8,
	BUMP7 = 9,
	SRTG = 10,
};

enum TexGenSrc {
	POSITION = 0,
	/// <summary>
	/// Useful for Environment mapping
	/// </summary>
	NORMAL = 1,
	BINORMAL = 2,
	TANGENT = 3,
	/// <summary>
	/// Mostly the default option
	/// </summary>
	UVCOORD0 = 4,
	UVCOORD1 = 5,
	UVCOORD2 = 6,
	UVCOORD3 = 7,
	UVCOORD4 = 8,
	UVCOORD5 = 9,
	UVCOORD6 = 10,
	UVCOORD7 = 11,
	TEXCOORD0 = 12,
	TEXCOORD1 = 13,
	TEXCOORD2 = 14,
	TEXCOORD3 = 15,
	TEXCOORD4 = 16,
	TEXCOORD5 = 17,
	TEXCOORD6 = 18,
	COLOR0 = 19,
	COLOR1 = 20,
};

enum TexGenMatrix {
	/// <summary>
	/// Force using an IDENTITY matrix
	/// </summary>
	IDENTITY = 60,
	/// <summary>
	/// Use Texture Matrix 0
	/// </summary>
	TEXMTX0 = 30,
	/// <summary>
	/// Use Texture Matrix 1
	/// </summary>
	TEXMTX1 = 33,
	/// <summary>
	/// Use Texture Matrix 2
	/// </summary>
	TEXMTX2 = 36,
	/// <summary>
	/// Use Texture Matrix 3
	/// </summary>
	TEXMTX3 = 39,
	/// <summary>
	/// Use Texture Matrix 4
	/// </summary>
	TEXMTX4 = 42,
	/// <summary>
	/// Use Texture Matrix 5
	/// </summary>
	TEXMTX5 = 45,
	/// <summary>
	/// Use Texture Matrix 6
	/// </summary>
	TEXMTX6 = 48,
	/// <summary>
	/// Use Texture Matrix 7
	/// </summary>
	TEXMTX7 = 51,
	/// <summary>
	/// Use Texture Matrix 8
	/// </summary>
	TEXMTX8 = 54,
	/// <summary>
	/// Use Texture Matrix 9
	/// </summary>
	TEXMTX9 = 57,

	/* Clever games can use PNMTX as inputs to texgen. */
	//We don't need to support these...
	PNMTX0 = 0,
	PNMTX1 = 3,
	PNMTX2 = 6,
	PNMTX3 = 9,
	PNMTX4 = 12,
	PNMTX5 = 15,
	PNMTX6 = 18,
	PNMTX7 = 21,
	PNMTX8 = 24,
	PNMTX9 = 27,
};

enum TexMtxMapMode {
	NONE = 0x00,
	// Uses "Basic" conventions, no -1...1 remap.
	ENVIRONMENT_BASIC = 0x01,
	PROJECTION_BASIC = 0x02,
	VIEWPROJECTION_BASIC = 0x03,

	// Unknown: 0x04, 0x05. No known uses.
	
	// Uses "Old" conventions, remaps translation in fourth component
	ENVIRONMENT_OLD = 0x06,

	// Uses "New" conventions, remaps translation in third component
	ENVIRONMENT = 0x07,
	PROJECTION = 0x08,
	VIEWPROJECTION = 0x09,

	// Environment map, but based on a custom effect matrix instead of the default view
	// matrix. Used by certain actors in Wind Waker, like zouK1 in Master Sword Chamber.
	ENVIRONMENT_OLD_EFFECTMTX = 0x0A,
	ENVIRONMENT_EFFECTMTX = 0x0B,
};

#endif /* __MAT3_H */