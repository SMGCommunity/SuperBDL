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

void writeVector2f(union Vector2f vec);
void writeVector3f(union Vector3f vec);
void writeVector3s(union Vector3s vec);
void writeVector4f(union Vector4f vec);
void writeVector4uc(union Vector4uc vec);
void writeVector4s(union Vector4s vec);

#endif /* __VECTOR_H */