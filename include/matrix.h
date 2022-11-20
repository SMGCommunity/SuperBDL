#ifndef __MATRIX_H
#define __MATRIX_H

union Matrix2x3f
{
	struct {
		float m00, m01, m02;
		float m10, m11, m12;
	};
	float values[2][3];
};
union Matrix2x4f
{
	struct {
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
	};
	float values[2][4];
};

union Matrix4x4f
{
	struct {
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;
	};
	float values[4][4];
};

void writeMatrix2x3f(union Matrix2x3f matrix);
void writeMatrix2x4f(union Matrix2x4f matrix);
void writeMatrix4x4f(union Matrix4x4f matrix);

#endif /* __MATRIX_H */