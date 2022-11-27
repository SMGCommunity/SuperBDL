#ifndef __VECTOR_H
#define __VECTOR_H

union Vector2f
{
	struct {
		float x, y;
	};
	float values[2];
};

union Vector3f
{
	struct {
		float x, y, z;
	};
	float values[3];
};
union Vector3s
{
	struct {
		short x, y, z;
	};
	short values[3];
};

union Vector4f
{
	struct {
		float x, y, z, w;
	};
	float values[4];
};
union Vector4uc
{
	struct {
		unsigned char r, g, b, a;
	};
	unsigned char values[4];
	unsigned int value;
};
union Vector4s
{
	struct {
		short r, g, b, a;
	};
	short values[4];
};


/// <summary>
/// The color WHITE
/// </summary>
const static union Vector4uc COLOR_WHITE = { .r = 255, .g = 255, .b = 255, .a = 255 };
/// <summary>
/// The color BLACK
/// </summary>
const static union Vector4uc COLOR_BLACK = { .r = 0, .g = 0, .b = 0, .a = 255 };
/// <summary>
/// The color.... of Transperant Black. According to Jasper the GameCube uses this as a default, and since the Wii also uses TEV, that means the Wii also uses this color as default
/// </summary>
const static union Vector4uc COLOR_DEFAULT = { .r = 0, .g = 0, .b = 0, .a = 0 };

/// <summary>
/// The default TEV Register color... basically COLOR_DEFAULT but with shorts instead
/// </summary>
const static union Vector4s COLOR_REGISTER_DEFAULT = { .r = 0, .g = 0, .b = 0, .a = 0 };
#endif /* __VECTOR_H */
