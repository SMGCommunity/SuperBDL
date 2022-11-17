#ifndef __MAT3_H
#define __MAT3_H

#include <stdbool.h>
#include <bmd.h>
#include <vector.h>
#include <matrix.h>

#define MAGIC_MAT3 "MAT3"

bool writeMAT3(const struct aiScene *data);





typedef struct J3DMaterial
{
	/// <summary>
	/// Name of the material. encoded in SHIFT-JIS
	/// </summary>
	char* Name;
	/// <summary>
	/// 0x01: OPA (Opaque), 0x02: EDG (TexEdge / Masked), 0x04: XLU (Translucent)
	/// </summary>
	unsigned char MaterialMode;

	bool EnableDithering;
	struct JUTTexture* Textures[8]; //The TEX1 section will hold the actual instances of these. However if TEX1 doesn't have a texture that's pointed to here, we'll add it to TEX1. Genius??
	struct TextureMatrix* TextureMatricies[10];
	union Matrix2x4f IndirectTextureMatricies[3];
	union Vector4uc ColorMatRegs[2];
	union Vector4uc ColorAmbRegs[2];
	/// <summary>
	/// KONST Colours are clamped 0 to 255.
	/// </summary>
	union Vector4uc ColorConstants[4];

	/// <summary>
	/// TEV Colours aren't clamped 0 to 255.
	/// </summary>
	union Vector4s ColorRegisters[4];
	struct Fog* FogInfo;

	//GXMaterial
	enum GXCullMode Culling;

	struct LightChannelControl* LightChannels[2];
	struct TextureGenerator* TextureGenerators[8];

	struct TextureEnvironmentStage* TEVStages[16];
	
	struct IndirectTextureStage* IndirectStages[4];
	
	struct AlphaTest* AlphaTest;
	struct Blend* BlendInfo;
	struct NBT* NBTScale;

} Material;

struct TextureMatrix
{
	enum TexMtxMapping Mode;
	bool IsMaya;
	enum TexMtxProjection Projection;

	union Vector3f Center; //Z is unused
	union Vector2f Scale;
	float Rotation;
	union Vector2f Translation;
	union Matrix4x4f EffectMatrix;
};

struct LightChannelControl
{
	struct ColorChannelControl *Color, *Alpha;
};

struct ColorChannelControl
{
	bool LightingEnabled = false;
	enum GXColorSource MaterialColorSource = SOURCE_VERTEX;
	enum GXColorSource AmbientColorSource = SOURCE_VERTEX;
	unsigned char LightMask = 0;
	enum GXDiffuseFunction DiffuseFunc = DIFFUSE_NONE;
	enum GXAttenuationFunction AttenuationFunc = ATTENUATION_NONE;
};

struct TextureGenerator
{
	enum GXTexGenType Type;
	enum GXTexGenSrc Source;
	enum GXTexGenMatrix Matrix;
	bool Normalize;
	enum GXPostTexGenMatrix PostMatrix;
};

typedef struct TextureEnvironmentStage
{
	enum GXCombineColor ColorInA;
	enum GXCombineColor ColorInB;
	enum GXCombineColor ColorInC;
	enum GXCombineColor ColorInD;
	enum GXTevOp ColorOperation;
	enum GXTevBias ColorBias;
	enum GXTevScale ColorScale;
	bool ColorClamp;
	enum GXRegister ColorOutRegister;

	enum GXCombineAlpha AlphaInA;
	enum GXCombineAlpha AlphaInB;
	enum GXCombineAlpha AlphaInC;
	enum GXCombineAlpha AlphaInD;
	enum GXTevOp AlphaOperation;
	enum GXTevBias AlphaBias;
	enum GXTevScale AlphaScale;
	bool AlphaClamp;
	enum GXRegister AlphaOutRegister;

	//TevOrder
	enum GXTexID TextureCoordID; //See, these two have the same values despite normally being different enums...
	enum GXTexID TextureMapID; //So I just merged them to use a single enum. We'll see if that gives us issues later...
	enum GXRasterizerChannelID ChannelID;

	//Constant Selections
	enum GXConstantColor ConstantColorSelection;
	enum GXConstantAlpha ConstantAlphaSelection;

	struct SwapTable* RasterizerSwapTable;
	struct SwapTable* TextureSwapTable;

	//Indirect Stage
	struct IndirectTextureStage* IndirectStagePtr = NULL;
	enum GXIndirectTextureFormat IndirectTexFormat = IND_TEXFMT_EIGHT_BIT;
	enum GXIndirectTextureBias IndirectTexBias = IND_TEXBIAS_NONE;
	enum GXIndirectTextureAlpha IndirectTexAlpha = IND_ALPHA_OFF;
	enum GXIndirectTextureMatrixID IndirectTexMtxID = INDMTX_OFF;
	enum GXIndirectTextureWrap IndirectTexWrapS = INDWRAP_OFF;
	enum GXIndirectTextureWrap IndirectTexWrapT = INDWRAP_OFF;
	bool IndirectAddPrevious = false;
	bool IndirectUseOriginalLoD = false;
} TEVStage;

struct SwapTable
{
	enum GXTevColorChannel
		RED = CHANNEL_RED,
		GREEN = CHANNEL_GREEN,
		BLUE = CHANNEL_BLUE,
		ALPHA = CHANNEL_ALPHA;
};

struct IndirectTextureStage
{
	enum GXTexID TexCoordID;
	enum GXTexID TexMapID;
	enum GXIndirectScale ScaleS;
	enum GXIndirectScale ScaleT;
};

struct AlphaTest
{
	enum GXAlphaOp Operation;
	enum GXCompareType CompareA; //Pixel?
	unsigned char ReferenceA;
	enum GXCompareType CompareB; //EFB??
	unsigned char ReferenceB;
};

struct Blend
{
	bool EnableDepthTest;
	/// Always PixelAlpha <op> EFB
	enum GXCompareType DepthFunction;
	bool WriteToZBuffer;

	enum GXBlendMode BlendMode;
	enum GXBlendFactor BlendSourceFactor;
	enum GXBlendFactor BlendDestFactor;
	enum GXLogicOp BlendLogicOperation;
};

struct Fog
{
	enum GXFogType Type = FOGTYPE_NONE;
	bool AdjustEnabled = false;
	unsigned short AdjustCenter;
	float StartZ, EndZ, NearZ, FarZ;
	union Vector4uc Color; //set to #00000000
	unsigned short AdjustTable[10];
};

struct NBT
{
	///what the heck even is this?
	unsigned char UNKNOWN;
	union Vector3f Scale;
};



#pragma region GX_Enums
enum TexMtxMapping
{
	/// <summary>
	/// No texture mapping
	/// </summary>
	TEXMTXMAP_NONE = 0x00,
	/// <summary>
	/// Uses "Basic" conventions, no -1...1 remap.
	/// </summary>
	TEXMTXMAP_BASIC_ENVIRONMENT_MAP = 0x01,
	/// <summary>
	/// Uses "Basic" conventions, no -1...1 remap.
	/// </summary>
	TEXMTXMAP_BASIC_PROJECTION_MAP = 0x02,
	/// <summary>
	/// Uses "Basic" conventions, no -1...1 remap.
	/// </summary>
	TEXMTXMAP_BASIC_VIEW_PROJECTION_MAP = 0x03,
	/// <summary>
	/// Unknown. No known uses. Here for completion.
	/// </summary>
	TEXMTXMAP_UNKNOWN1 = 0x04,
	/// <summary>
	/// Unknown. No known uses. Here for completion.
	/// </summary>
	TEXMTXMAP_UNKNOWN2 = 0x05,
	/// <summary>
	/// Uses "Old" conventions, remaps translation in fourth component of the matrix (Very common in SMG/2)
	/// </summary>
	TEXMTXMAP_OLD_ENVIRONMENT_MAP = 0x06,
	/// <summary>
	/// Uses "New" conventions, remaps translation in third component
	/// </summary>
	TEXMTXMAP_ENVIRONMENT_MAP = 0x07,
	/// <summary>
	/// Uses "New" conventions, remaps translation in third component
	/// </summary>
	TEXMTXMAP_PROJECTION_MAP = 0x08,
	/// <summary>
	/// Uses "New" conventions, remaps translation in third component
	/// </summary>
	TEXMTXMAP_VIEW_PROJECTION_MAP = 0x09,
	/// <summary>
	/// Environment map, but based on a custom effect matrix instead of the default view matrix. (using "Old" conventions)
	/// </summary>
	TEXMTXMAP_OLD_EFFECT_ENVIRONMENT_MAP = 0x0A,
	/// <summary>
	/// Environment map, but based on a custom effect matrix instead of the default view matrix. (using "New" conventions)
	/// </summary>
	TEXMTXMAP_EFFECT_ENVIRONMENT_MAP = 0x0B,
};

enum TexMtxProjection {
	TEXMTXPROJ_MTX3x4 = 0,
	TEXMTXPROJ_MTX2x4 = 1,
};



enum GXCullMode
{
	/// <summary>
	/// Render both sides of the face
	/// </summary>
	CULL_NONE,
	/// <summary>
	/// Render only the back of the face
	/// </summary>
	CULL_FRONT,
	/// <summary>
	/// Render only the front of the face (Default)
	/// </summary>
	CULL_BACK,
	/// <summary>
	/// Render neither side of the face
	/// </summary>
	CULL_BOTH
};

enum GXColorSource
{
	/// <summary>
	/// Source colors from the model's Vertex Colors (Default)
	/// </summary>
	SOURCE_VERTEX,
	/// <summary>
	/// Source colors from the a single register
	/// </summary>
	SOURCE_REGISTER
};

enum GXDiffuseFunction
{
	DIFFUSE_NONE,
	DIFFUSE_SIGNED,
	DIFFUSE_CLAMPED
};

enum GXAttenuationFunction
{
	ATTENUATION_SPECULAR,
	ATTENUATION_SPOTLIGHT,
	ATTENUATION_NONE
};

enum GXTexGenType
{
	TEXGENTYPE_MTX3x4 = 0,
	TEXGENTYPE_MTX2x4 = 1,
	TEXGENTYPE_BUMP0 = 2,
	TEXGENTYPE_BUMP1 = 3,
	TEXGENTYPE_BUMP2 = 4,
	TEXGENTYPE_BUMP3 = 5,
	TEXGENTYPE_BUMP4 = 6,
	TEXGENTYPE_BUMP5 = 7,
	TEXGENTYPE_BUMP6 = 8,
	TEXGENTYPE_BUMP7 = 9,
	TEXGENTYPE_SRTG = 10
};

enum GXTexGenSrc
{
	TEXGENSRC_POSITION = 0,
	TEXGENSRC_NORMAL = 1,
	TEXGENSRC_BINORMAL = 2,
	TEXGENSRC_TANGENT = 3,
	TEXGENSRC_TEXTURE0 = 4,
	TEXGENSRC_TEXTURE1 = 5,
	TEXGENSRC_TEXTURE2 = 6,
	TEXGENSRC_TEXTURE3 = 7,
	TEXGENSRC_TEXTURE4 = 8,
	TEXGENSRC_TEXTURE5 = 9,
	TEXGENSRC_TEXTURE6 = 10,
	TEXGENSRC_TEXTURE7 = 11,
	TEXGENSRC_TEXCOORD0 = 12,
	TEXGENSRC_TEXCOORD1 = 13,
	TEXGENSRC_TEXCOORD2 = 14,
	TEXGENSRC_TEXCOORD3 = 15,
	TEXGENSRC_TEXCOORD4 = 16,
	TEXGENSRC_TEXCOORD5 = 17,
	TEXGENSRC_TEXCOORD6 = 18,
	TEXGENSRC_COLOR0 = 19,
	TEXGENSRC_COLOR1 = 20,
};

enum GXTexGenMatrix
{
	TEXGENMTX_IDENTITY = 60,
	TEXGENMTX_TEXTUREMATRIX0 = 30,
	TEXGENMTX_TEXTUREMATRIX1 = 33,
	TEXGENMTX_TEXTUREMATRIX2 = 36,
	TEXGENMTX_TEXTUREMATRIX3 = 39,
	TEXGENMTX_TEXTUREMATRIX4 = 42,
	TEXGENMTX_TEXTUREMATRIX5 = 45,
	TEXGENMTX_TEXTUREMATRIX6 = 48,
	TEXGENMTX_TEXTUREMATRIX7 = 51,
	TEXGENMTX_TEXTUREMATRIX8 = 54,
	TEXGENMTX_TEXTUREMATRIX9 = 57,

	/* Clever games can use PNMTX as inputs to texgen. */
	TEXGENMTX_PNMTX0 = 0,
	TEXGENMTX_PNMTX1 = 3,
	TEXGENMTX_PNMTX2 = 6,
	TEXGENMTX_PNMTX3 = 9,
	TEXGENMTX_PNMTX4 = 12,
	TEXGENMTX_PNMTX5 = 15,
	TEXGENMTX_PNMTX6 = 18,
	TEXGENMTX_PNMTX7 = 21,
	TEXGENMTX_PNMTX8 = 24,
	TEXGENMTX_PNMTX9 = 27,
};

enum GXPostTexGenMatrix
{
	POSTTEXGENMTX_TEXTUREMATRIX0 = 64,
	POSTTEXGENMTX_TEXTUREMATRIX1 = 67,
	POSTTEXGENMTX_TEXTUREMATRIX2 = 70,
	POSTTEXGENMTX_TEXTUREMATRIX3 = 73,
	POSTTEXGENMTX_TEXTUREMATRIX4 = 76,
	POSTTEXGENMTX_TEXTUREMATRIX5 = 79,
	POSTTEXGENMTX_TEXTUREMATRIX6 = 82,
	POSTTEXGENMTX_TEXTUREMATRIX7 = 85,
	POSTTEXGENMTX_TEXTUREMATRIX8 = 88,
	POSTTEXGENMTX_TEXTUREMATRIX9 = 91,
	POSTTEXGENMTX_TEXTUREMATRIX10 = 94,
	POSTTEXGENMTX_TEXTUREMATRIX11 = 97,
	POSTTEXGENMTX_TEXTUREMATRIX12 = 100,
	POSTTEXGENMTX_TEXTUREMATRIX13 = 103,
	POSTTEXGENMTX_TEXTUREMATRIX14 = 106,
	POSTTEXGENMTX_TEXTUREMATRIX15 = 109,
	POSTTEXGENMTX_TEXTUREMATRIX16 = 112,
	POSTTEXGENMTX_TEXTUREMATRIX17 = 115,
	POSTTEXGENMTX_TEXTUREMATRIX18 = 118,
	POSTTEXGENMTX_TEXTUREMATRIX19 = 121,
	POSTTEXGENMTX_IDENTITY = 125,
};

enum GXCombineColor
{
	/// <summary>
	/// Use the color value from previous TEV stage
	/// </summary>
	COMBINECOLOR_COLORPREVIOUS = 0x00,
	/// <summary>
	/// Use the alpha value from previous TEV stage
	/// </summary>
	COMBINECOLOR_ALPHAPREVIOUS = 0x01,
	/// <summary>
	/// Use the color value from the color/output register 0
	/// </summary>
	COMBINECOLOR_COLORREGISTER0 = 0x02,
	/// <summary>
	/// Use the alpha value from the color/output register 0
	/// </summary>
	COMBINECOLOR_ALPHAREGISTER0 = 0x03,
	/// <summary>
	/// Use the color value from the color/output register 1
	/// </summary>
	COMBINECOLOR_COLORREGISTER1 = 0x04,
	/// <summary>
	/// Use the alpha value from the color/output register 1
	/// </summary>
	COMBINECOLOR_ALPHAREGISTER1 = 0x05,
	/// <summary>
	/// Use the color value from the color/output register 2
	/// </summary>
	COMBINECOLOR_COLORREGISTER2 = 0x06,
	/// <summary>
	/// Use the alpha value from the color/output register 2
	/// </summary>
	COMBINECOLOR_ALPHAREGISTER2 = 0x07,
	/// <summary>
	/// Use the color value from texture
	/// </summary>
	COMBINECOLOR_TEXTURECOLOR = 0x08,
	/// <summary>
	/// Use the alpha value from texture
	/// </summary>
	COMBINECOLOR_TEXTUREALPHA = 0x09,
	/// <summary>
	/// Use the color value from rasterizer
	/// </summary>
	COMBINECOLOR_RASTERIZERCOLOR = 0x0A,
	/// <summary>
	/// Use the alpha value from rasterizer
	/// </summary>
	COMBINECOLOR_RASTERIZERALPHA = 0x0B,
	/// <summary>
	/// Use to pass one as value
	/// </summary>
	COMBINECOLOR_ONE = 0x0C,
	/// <summary>
	/// Use to pass 0.5 as value
	/// </summary>
	COMBINECOLOR_HALF = 0x0D,
	/// <summary>
	/// Use the selected CONSTANT value
	/// </summary>
	COMBINECOLOR_CONSTANTSELECTION = 0x0E,
	/// <summary>
	/// Use to pass zero as value
	/// </summary>
	COMBINECOLOR_ZERO = 0x0F
};

enum GXCombineAlpha
{
	/// <summary>
	/// Use the alpha value from previous TEV stage
	/// </summary>
	COMBINEALPHA_ALPHAPREVIOUS = 0x00,
	/// <summary>
	/// Use the alpha value from the color/output register 0
	/// </summary>
	COMBINEALPHA_ALPHAREGISTER0 = 0x01,
	/// <summary>
	/// Use the alpha value from the color/output register 0
	/// </summary>
	COMBINEALPHA_ALPHAREGISTER1 = 0x02,
	/// <summary>
	/// Use the alpha value from the color/output register 0
	/// </summary>
	COMBINEALPHA_ALPHAREGISTER2 = 0x03,
	/// <summary>
	/// Use the alpha value from texture
	/// </summary>
	COMBINEALPHA_TEXTUREALPHA = 0x04,
	/// <summary>
	/// Use the alpha value from rasterizer
	/// </summary>
	COMBINEALPHA_RASTERIZERALPHA = 0x05,
	/// <summary>
	/// Use the selected CONSTANT value
	/// </summary>
	COMBINEALPHA_CONSTANTSELECTION = 0x06,
	/// <summary>
	/// Use to pass zero as value
	/// </summary>
	COMBINEALPHA_ZERO = 0x07
};

enum GXTevOp
{
	OPERATION_ADD = 0,
	OPERATION_SUB = 1,
	OPERATION_COMP_R8_GT = 8,
	OPERATION_COMP_R8_EQ = 9,
	OPERATION_COMP_GR16_GT = 10,
	OPERATION_COMP_GR16_EQ = 11,
	OPERATION_COMP_BGR24_GT = 12,
	OPERATION_COMP_BGR24_EQ = 13,
	OPERATION_COMP_RGB8_GT, OPERATION_COMP_A8_GT = 14,
	OPERATION_COMP_RGB8_EQ, OPERATION_COMP_A8_EQ = 15
};

enum GXTevBias
{
	BIAS_ZERO = 0,
	BIAS_ADDHALF = 1,
	BIAS_SUBHALF = 2,
	/// <summary>
	/// Used to denote the compare ops to the HW.
	/// </summary>
	BIAS_HWB_COMPARE = 3
};

enum GXTevScale
{
	SCALE_SCALE_1 = 0,
	SCALE_SCALE_2 = 1,
	SCALE_SCALE_4 = 2,
	SCALE_DIVIDE_2 = 3,

	// Used to denote the width of the compare op.
	SCALE_HWB_R8 = 0,
	SCALE_HWB_GR16 = 1,
	SCALE_HWB_BGR24 = 2,
	SCALE_HWB_RGB8 = 3,
};

enum GXRegister
{
	REGISTER_PREVIOUS, REGISTER_3 = 0,
	REGISTER_0 = 1,
	REGISTER_1 = 2,
	REGISTER_2 = 3,
};

enum GXTexID
{
	TEXTURE_0 = 0,
	TEXTURE_1 = 1,
	TEXTURE_2 = 2,
	TEXTURE_3 = 3,
	TEXTURE_4 = 4,
	TEXTURE_5 = 5,
	TEXTURE_6 = 6,
	TEXTURE_7 = 7,
	TEXTURE_NULL = 0xFF,
};

enum GXRasterizerChannelID
{
	RASTERIZER_COLOR0_ALPHA0 = 0x00,
	RASTERIZER_COLOR1_ALPHA1 = 0x01,
	RASTERIZER_ALPHA_BUMP = 0x05,

	/// <summary>
	/// Also known as ALPHA_BUMP_N
	/// </summary>
	RASTERIZER_ALPHA_NORMAL = 0x06,
	RASTERIZER_ZERO = 0x07
};

enum GXConstantColor
{
	CONST_COLOR_ONE = 0x00, /*!< constant 1.0 */
	CONST_COLOR_SEVEN_EIGHTHS = 0x01, /*!< constant 7/8 */
	CONST_COLOR_SIX_EIGHTHS = 0x02, /*!< constant 6/8 */
	CONST_COLOR_FIVE_EIGHTHS = 0x03, /*!< constant 5/8 */
	CONST_COLOR_FOUR_EIGHTHS = 0x04, /*!< constant 4/8 */
	CONST_COLOR_THREE_EIGHTHS = 0x05, /*!< constant 3/8 */
	CONST_COLOR_TWO_EIGHTHS = 0x06, /*!< constant 2/8 */
	CONST_COLOR_ONE_EIGHTH = 0x07, /*!< constant 1/8 */

	CONST_COLOR_RGB0 = 0x0C, /*!< K0[RGB] register */
	CONST_COLOR_RGB1 = 0x0D, /*!< K1[RGB] register */
	CONST_COLOR_RGB2 = 0x0E, /*!< K2[RGB] register */
	CONST_COLOR_RGB3 = 0x0F, /*!< K3[RGB] register */

	CONST_COLOR_RRR0 = 0x10, /*!< K0[RRR] register */
	CONST_COLOR_RRR1 = 0x11, /*!< K1[RRR] register */
	CONST_COLOR_RRR2 = 0x12, /*!< K2[RRR] register */
	CONST_COLOR_RRR3 = 0x13, /*!< K3[RRR] register */

	CONST_COLOR_GGG0 = 0x14, /*!< K0[GGG] register */
	CONST_COLOR_GGG1 = 0x15, /*!< K1[GGG] register */
	CONST_COLOR_GGG2 = 0x16, /*!< K2[GGG] register */
	CONST_COLOR_GGG3 = 0x17, /*!< K3[GGG] register */

	CONST_COLOR_BBB0 = 0x18, /*!< K0[BBB] register */
	CONST_COLOR_BBB1 = 0x19, /*!< K1[BBB] register */
	CONST_COLOR_BBB2 = 0x1A, /*!< K2[BBB] register */
	CONST_COLOR_BBB3 = 0x1B, /*!< K3[RBB] register */

	CONST_COLOR_AAA0 = 0x1C, /*!< K0[AAA] register */
	CONST_COLOR_AAA1 = 0x1D, /*!< K1[AAA] register */
	CONST_COLOR_AAA2 = 0x1E, /*!< K2[AAA] register */
	CONST_COLOR_AAA3 = 0x1F, /*!< K3[AAA] register */
};

enum GXConstantAlpha
{
	CONST_ALPHA_ONE = 0x00, /*!< constant 1.0 */
	CONST_ALPHA_SEVEN_EIGHTHS = 0x01, /*!< constant 7/8 */
	CONST_ALPHA_SIX_EIGHTHS = 0x02, /*!< constant 6/8 */
	CONST_ALPHA_FIVE_EIGHTHS = 0x03, /*!< constant 5/8 */
	CONST_ALPHA_FOUR_EIGHTHS = 0x04, /*!< constant 4/8 */
	CONST_ALPHA_THREE_EIGHTHS = 0x05, /*!< constant 3/8 */
	CONST_ALPHA_TWO_EIGHTHS = 0x06, /*!< constant 2/8 */
	CONST_ALPHA_ONE_EIGHTH = 0x07, /*!< constant 1/8 */

	CONST_ALPHA_R0 = 0x10, /*!< K0[R] register */
	CONST_ALPHA_R1 = 0x11, /*!< K1[R] register */
	CONST_ALPHA_R2 = 0x12, /*!< K2[R] register */
	CONST_ALPHA_R3 = 0x13, /*!< K3[R] register */

	CONST_ALPHA_G0 = 0x14, /*!< K0[G] register */
	CONST_ALPHA_G1 = 0x15, /*!< K1[G] register */
	CONST_ALPHA_G2 = 0x16, /*!< K2[G] register */
	CONST_ALPHA_G3 = 0x17, /*!< K3[G] register */

	CONST_ALPHA_B0 = 0x18, /*!< K0[B] register */
	CONST_ALPHA_B1 = 0x19, /*!< K1[B] register */
	CONST_ALPHA_B2 = 0x1A, /*!< K2[B] register */
	CONST_ALPHA_B3 = 0x1B, /*!< K3[R] register */

	CONST_ALPHA_A0 = 0x1C, /*!< K0[A] register */
	CONST_ALPHA_A1 = 0x1D, /*!< K1[A] register */
	CONST_ALPHA_A2 = 0x1E, /*!< K2[A] register */
	CONST_ALPHA_A3 = 0x1F, /*!< K3[A] register */
};

enum GXTevColorChannel
{
	CHANNEL_RED = 0x00,
	CHANNEL_GREEN = 0x01,
	CHANNEL_BLUE = 0x02,
	CHANNEL_ALPHA = 0x03,
};

enum GXIndirectScale
{
	IND_SCALE_ONE = 0,
	IND_SCALE_TWO = 1,
	IND_SCALE_FOUR = 2,
	IND_SCALE_EIGHT = 3,
	IND_SCALE_SIXTEEN = 4,
	IND_SCALE_THIRTY_TWO = 5,
	IND_SCALE_SIXTY_FOUR = 6,
	IND_SCALE_HUNDRED_TWENTY_EIGHT = 7,
	IND_SCALE_TWO_HUNDRED_FIFTY_SIX = 8,
};

enum GXIndirectTextureFormat
{
	IND_TEXFMT_EIGHT_BIT = 0x00,
	IND_TEXFMT_FIVE_BIT = 0x01,
	IND_TEXFMT_FOUR_BIT = 0x02,
	IND_TEXFMT_THREE_BIT = 0x03,
};

enum GXIndirectTextureBias
{
	IND_TEXBIAS_NONE = 0,
	IND_TEXBIAS_S = 1,
	IND_TEXBIAS_T = 2,
	IND_TEXBIAS_ST = 3,
	IND_TEXBIAS_U = 4,
	IND_TEXBIAS_SU = 5,
	IND_TEXBIAS_TU = 6,
	IND_TEXBIAS_STU = 7
};

enum GXIndirectTextureAlpha
{
	IND_ALPHA_OFF = 0,
	IND_ALPHA_S = 1,
	IND_ALPHA_T = 2,
	IND_ALPHA_U = 3
};

enum GXIndirectTextureMatrixID
{
	INDMTX_OFF = 0,
	INDMTX_ZERO = 1,
	INDMTX_ONE = 2,
	INDMTX_TWO = 3,

	INDMTX_S_ZERO = 5,
	INDMTX_S_ONE = 6,
	INDMTX_S_TWO = 7,

	INDMTX_T_ZERO = 9,
	INDMTX_T_ONE = 10,
	INDMTX_T_TWO = 11
};

enum GXIndirectTextureWrap
{
	INDWRAP_OFF = 0,
	INDWRAP_TWO_HUNDRED_FIFTY_SIX = 1,
	INDWRAP_HUNDRED_TWENTY_EIGHT = 2,
	INDWRAP_SIXTY_FOUR = 3,
	INDWRAP_THIRTY_TWO = 4,
	INDWRAP_SIXTEEN = 5,
	INDWRAP_ZERO = 6
};

enum GXAlphaOp
{
	ALPHAOP_AND = 0,
	ALPHAOP_OR = 1,
	ALPHAOP_XOR = 2,
	ALPHAOP_XNOR = 3,
};

enum GXCompareType
{
	ALPHACOMP_NEVER = 0,
	ALPHACOMP_LESS = 1,
	ALPHACOMP_EQUAL = 2,
	ALPHACOMP_LESSEQUAL = 3,
	ALPHACOMP_GREATER = 4,
	ALPHACOMP_NOTEQUAL = 5,
	ALPHACOMP_GREATEQUAL = 6,
	ALPHACOMP_ALWAYS = 7,
};

enum GXBlendMode
{
	BLENDOP_NONE = 0,
	BLENDOP_BLEND = 1,
	BLENDOP_LOGIC = 2,
	BLENDOP_SUBTRACT = 3
};

enum GXBlendFactor
{
	BLENDFACTOR_ZERO = 0,
	BLENDFACTOR_ONE,
	BLENDFACTOR_SOURCECOLOR,
	BLENDFACTOR_SOURCECOLOR_INVERSE,
	BLENDFACTOR_SOURCEALPHA,
	BLENDFACTOR_SOURCEALPHA_INVERSE,
	BLENDFACTOR_DESTALPHA,
	BLENDFACTOR_DESTALPHA_INVERSE = 7,
};

enum GXLogicOp
{
	LOGICOP_CLEAR = 0,
	LOGICOP_AND = 1,
	LOGICOP_REVAND = 2,
	LOGICOP_COPY = 3,
	LOGICOP_INVAND = 4,
	LOGICOP_NOOP = 5,
	LOGICOP_XOR = 6,
	LOGICOP_OR = 7,
	LOGICOP_NOR = 8,
	LOGICOP_EQUIV = 9,
	LOGICOP_INV = 10,
	LOGICOP_REVOR = 11,
	LOGICOP_INVCOPY = 12,
	LOGICOP_INVOR = 13,
	LOGICOP_NAND = 14,
	LOGICOP_SET = 15,
};

enum GXFogType
{
	FOGTYPE_NONE = 0x00,

	FOGTYPE_PERSP_LINEAR = 0x02,
	FOGTYPE_PERSP_EXPONENTIAL = 0x04,
	FOGTYPE_PERSP_QUADRATIC = 0x05,
	FOGTYPE_PERSP_EXPONENTIALREVERSE = 0x06,
	FOGTYPE_PERSP_QUADRATICREVERSE = 0x07,

	FOGTYPE_ORTHO_LINEAR = 0x0A,
	FOGTYPE_ORTHO_EXPONENTIAL = 0x0C,
	FOGTYPE_ORTHO_QUADRATIC = 0x0D,
	FOGTYPE_ORTHO_EXPONENTIALREVERSE = 0x0E,
	FOGTYPE_ORTHO_QUADRATICREVERSE = 0x0F,
};

#pragma endregion






#endif /* __MAT3_H */