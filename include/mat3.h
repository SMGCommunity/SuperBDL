#ifndef __MAT3_H
#define __MAT3_H

#include <stdbool.h>
#include <bmd.h>
#include <vector.h>
#include <matrix.h>

#define MAGIC_MAT3 "MAT3"

bool writeMAT3(const struct aiScene *data);


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
	/// <summary>
	/// For alpha channels, only set the A value
	/// </summary>
	Vector4uc diffuseColor, ambientColor;
};

//Ignore the Light colours section as it's useless

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

/// <summary>
/// There can only be 4 of these in a file
/// </summary>
struct IndirectTevStage
{
	TexCoordID texCoordID;
	TexMapID texMapID;

	IndTexScale scaleX, scaleY;

	Matrix2x3f matrix;
	float scale;
};

/// <summary>
/// There can only be 16 of these in a file
/// </summary>
struct TevStage
{
	//Color
	GXCombineColor colorInA;
	GXCombineColor colorInB;
	GXCombineColor colorInC;
	GXCombineColor colorInD;
	GXTevOp colorOperation;
	GXTevBias colorBias;
	GXTevScale colorScale;
	bool colorClamp;
	GXRegister colorOutRegister;

	//Alpha
	GXCombineAlpha alphaInA;
	GXCombineAlpha alphaInB;
	GXCombineAlpha alphaInC;
	GXCombineAlpha alphaInD;
	GXTevOp alphaOperation;
	GXTevBias alphaBias;
	GXTevScale alphaScale;
	bool alphaClamp;
	GXRegister alphaOutRegister;

	//TevOrder
	TexCoordID textureCoordID;
	TexMapID textureMapID;
	GXRasterizerChannelID channelID;

	//Constant Selections
	GXConstantColor ConstantColorSelection;
	GXConstantAlpha ConstantAlphaSelection;

	SwapTable RasterizerSwapTable;
	SwapTable TextureSwapTable;

	IndirectTevStage* indirectStagePtr;
};
struct SwapTable
{
	GXTevColorChannel RED, GREEN, BLUE, ALPHA;
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

enum TexCoordID {
	TEXCOORD0 = 0,
	TEXCOORD1 = 1,
	TEXCOORD2 = 2,
	TEXCOORD3 = 3,
	TEXCOORD4 = 4,
	TEXCOORD5 = 5,
	TEXCOORD6 = 6,
	TEXCOORD7 = 7,
	TEXCOORD_NULL = 0xFF,
};

enum TexMapID {
	TEXMAP0 = 0,
	TEXMAP1 = 1,
	TEXMAP2 = 2,
	TEXMAP3 = 3,
	TEXMAP4 = 4,
	TEXMAP5 = 5,
	TEXMAP6 = 6,
	TEXMAP7 = 7,
	TEXMAP_NULL = 0xFF,
};

enum IndTexScale {
	/// <summary>
	/// x1.0
	/// </summary>
	_1 = 0,
	_2 = 1,
	_4 = 2,
	_8 = 3,
	_16 = 4,
	_32 = 5,
	_64 = 6,
	_128 = 7,
	_256 = 8,
};

enum GXCombineColor
{
	/// <summary>
	/// Use the color value from previous TEV stage
	/// </summary>
	COLORPREVIOUS = 0x00,
	/// <summary>
	/// Use the alpha value from previous TEV stage
	/// </summary>
	ALPHAPREVIOUS = 0x01,
	/// <summary>
	/// Use the color value from the color/output register 0
	/// </summary>
	COLORREGISTER0 = 0x02,
	/// <summary>
	/// Use the alpha value from the color/output register 0
	/// </summary>
	ALPHAREGISTER0 = 0x03,
	/// <summary>
	/// Use the color value from the color/output register 1
	/// </summary>
	COLORREGISTER1 = 0x04,
	/// <summary>
	/// Use the alpha value from the color/output register 1
	/// </summary>
	ALPHAREGISTER1 = 0x05,
	/// <summary>
	/// Use the color value from the color/output register 2
	/// </summary>
	COLORREGISTER2 = 0x06, /*!<  */
	/// <summary>
	/// Use the alpha value from the color/output register 2
	/// </summary>
	ALPHAREGISTER2 = 0x07,
	/// <summary>
	/// Use the color value from texture
	/// </summary>
	TEXTURECOLOR = 0x08,
	/// <summary>
	/// Use the alpha value from texture
	/// </summary>
	TEXTUREALPHA = 0x09,
	/// <summary>
	/// Use the color value from rasterizer
	/// </summary>
	RASTERIZERCOLOR = 0x0A,
	/// <summary>
	/// Use the alpha value from rasterizer
	/// </summary>
	RASTERIZERALPHA = 0x0B,
	/// <summary>
	/// Use to pass one as value
	/// </summary>
	ONE = 0x0C,
	/// <summary>
	/// Use to pass 0.5 as value
	/// </summary>
	HALF = 0x0D,
	/// <summary>
	/// Use the selected CONSTANT value
	/// </summary>
	CONSTANTSELECTION = 0x0E,
	/// <summary>
	/// Use to pass zero as value
	/// </summary>
	ZERO = 0x0F
};
enum GXCombineAlpha
{
	/// <summary>
	/// Use the alpha value from previous TEV stage
	/// </summary>
	ALPHAPREVIOUS = 0x00,
	/// <summary>
	/// Use the alpha value from the color/output register 0
	/// </summary>
	ALPHAREGISTER0 = 0x01,
	/// <summary>
	/// Use the alpha value from the color/output register 0
	/// </summary>
	ALPHAREGISTER1 = 0x02,
	/// <summary>
	/// Use the alpha value from the color/output register 0
	/// </summary>
	ALPHAREGISTER2 = 0x03,
	/// <summary>
	/// Use the alpha value from texture
	/// </summary>
	TEXTUREALPHA = 0x04,
	/// <summary>
	/// Use the alpha value from rasterizer
	/// </summary>
	RASTERIZERALPHA = 0x05,
	/// <summary>
	/// Use the selected CONSTANT value
	/// </summary>
	CONSTANTSELECTION = 0x06,
	/// <summary>
	/// Use to pass zero as value
	/// </summary>
	ZERO = 0x07
};
enum GXTevOp
{
	ADD = 0,
	SUB = 1,
	COMP_R8_GT = 8,
	COMP_R8_EQ = 9,
	COMP_GR16_GT = 10,
	COMP_GR16_EQ = 11,
	COMP_BGR24_GT = 12,
	COMP_BGR24_EQ = 13,
	COMP_RGB8_GT, COMP_A8_GT = 14,
	COMP_RGB8_EQ, COMP_A8_EQ = 15,
};
enum GXTevBias
{
	ZERO = 0,
	ADDHALF = 1,
	SUBHALF = 2,

	// Used to denote the compare ops to the HW.
	HWB_COMPARE = 3,
};
enum GXTevScale
{
	SCALE_1, SCALE_NONE = 0,
	SCALE_2 = 1,
	SCALE_4 = 2,
	DIVIDE_2 = 3,

	// Used to denote the width of the compare op.
	HWB_R8 = 0,
	HWB_GR16 = 1,
	HWB_BGR24 = 2,
	HWB_RGB8 = 3,
};
enum GXRegister
{
	PREVIOUS, REGISTER3 = 0x00,
	REGISTER0 = 0x01,
	REGISTER1 = 0x02,
	REGISTER2 = 0x03,
};
enum GXRasterizerChannelID
{
	COLOR0_ALPHA0 = 0x00,
	COLOR1_ALPHA1 = 0x01,
	ALPHA_BUMP = 0x05,
	// Also known as ALPHA_BUMP_N
	ALPHA_NORMAL = 0x06,
	ZERO = 0x07,
};
enum GXConstantColor
{
	ONE = 0x00, /*!< constant 1.0 */
	SEVEN_EIGHTHS = 0x01, /*!< constant 7/8 */
	SIX_EIGHTHS = 0x02, /*!< constant 6/8 */
	FIVE_EIGHTHS = 0x03, /*!< constant 5/8 */
	FOUR_EIGHTHS = 0x04, /*!< constant 4/8 */
	THREE_EIGHTHS = 0x05, /*!< constant 3/8 */
	TWO_EIGHTHS = 0x06, /*!< constant 2/8 */
	ONE_EIGHTH = 0x07, /*!< constant 1/8 */

	COLOR_CONSTANT0 = 0x0C, /*!< K0[RGB] register */
	COLOR_CONSTANT1 = 0x0D, /*!< K1[RGB] register */
	COLOR_CONSTANT2 = 0x0E, /*!< K2[RGB] register */
	COLOR_CONSTANT3 = 0x0F, /*!< K3[RGB] register */

	RED_CONSTANT0 = 0x10, /*!< K0[RRR] register */
	RED_CONSTANT1 = 0x11, /*!< K1[RRR] register */
	RED_CONSTANT2 = 0x12, /*!< K2[RRR] register */
	RED_CONSTANT3 = 0x13, /*!< K3[RRR] register */

	GREEN_CONSTANT0 = 0x14, /*!< K0[GGG] register */
	GREEN_CONSTANT1 = 0x15, /*!< K1[GGG] register */
	GREEN_CONSTANT2 = 0x16, /*!< K2[GGG] register */
	GREEN_CONSTANT3 = 0x17, /*!< K3[GGG] register */

	BLUE_CONSTANT0 = 0x18, /*!< K0[BBB] register */
	BLUE_CONSTANT1 = 0x19, /*!< K1[BBB] register */
	BLUE_CONSTANT2 = 0x1A, /*!< K2[BBB] register */
	BLUE_CONSTANT3 = 0x1B, /*!< K3[RBB] register */

	ALPHA_CONSTANT0 = 0x1C, /*!< K0[AAA] register */
	ALPHA_CONSTANT1 = 0x1D, /*!< K1[AAA] register */
	ALPHA_CONSTANT2 = 0x1E, /*!< K2[AAA] register */
	ALPHA_CONSTANT3 = 0x1F, /*!< K3[AAA] register */
};
enum GXConstantAlpha
{
	ONE = 0x00, /*!< constant 1.0 */
	SEVEN_EIGHTHS = 0x01, /*!< constant 7/8 */
	SIX_EIGHTHS = 0x02, /*!< constant 6/8 */
	FIVE_EIGHTHS = 0x03, /*!< constant 5/8 */
	FOUR_EIGHTHS = 0x04, /*!< constant 4/8 */
	THREE_EIGHTHS = 0x05, /*!< constant 3/8 */
	TWO_EIGHTHS = 0x06, /*!< constant 2/8 */
	ONE_EIGHTH = 0x07, /*!< constant 1/8 */

	RED_CONSTANT0 = 0x10, /*!< K0[R] register */
	RED_CONSTANT1 = 0x11, /*!< K1[R] register */
	RED_CONSTANT2 = 0x12, /*!< K2[R] register */
	RED_CONSTANT3 = 0x13, /*!< K3[R] register */

	GREEN_CONSTANT0 = 0x14, /*!< K0[G] register */
	GREEN_CONSTANT1 = 0x15, /*!< K1[G] register */
	GREEN_CONSTANT2 = 0x16, /*!< K2[G] register */
	GREEN_CONSTANT3 = 0x17, /*!< K3[G] register */

	BLUE_CONSTANT0 = 0x18, /*!< K0[B] register */
	BLUE_CONSTANT1 = 0x19, /*!< K1[B] register */
	BLUE_CONSTANT2 = 0x1A, /*!< K2[B] register */
	BLUE_CONSTANT3 = 0x1B, /*!< K3[R] register */

	ALPHA_CONSTANT0 = 0x1C, /*!< K0[A] register */
	ALPHA_CONSTANT1 = 0x1D, /*!< K1[A] register */
	ALPHA_CONSTANT2 = 0x1E, /*!< K2[A] register */
	ALPHA_CONSTANT3 = 0x1F, /*!< K3[A] register */
};
enum GXTevColorChannel
{
	RED = 0x00,
	GREEN = 0x01,
	BLUE = 0x02,
	ALPHA = 0x03,
};
#endif /* __MAT3_H */