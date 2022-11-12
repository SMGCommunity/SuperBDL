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

void writeVector(Vector2f vec);
void writeVector(Vector3f vec);
void writeVector(Vector3s vec);
void writeVector(Vector4f vec);

#endif /* __VECTOR_H */