#include <stdbool.h>
#include <stdio.h>

#include "mat3.h"
#include "tex1.h"

bool readMAT3(FILE* fp, struct J3DMaterial** outputArray, unsigned int* elementCount)
{
	//if (!isMagicMatch(fp, MAGIC_MAT3))
		return false;


}

bool matcmp(struct J3DMaterial* mat1, struct J3DMaterial* mat2) {
	if (mat1 == mat2)
		return true;

	if (strcmp(mat1->Name, mat2->Name))
		return false;

	if (mat1->MaterialMode != mat2->MaterialMode)
		return false;

	if (mat1->EnableDithering != mat2->EnableDithering)
		return false;

	for (int i = 0; i < 8; ++i) {
		if (!texcmp(mat1->Textures[i], mat2->Textures[i]))
			return false;
	}

	for (int i = 0; i < 10; ++i) {
		if (memcmp(mat1->TextureMatricies, mat2->TextureMatricies,
		           sizeof (struct TextureMatrix)))
			return false;
	}

	for (int i = 0; i < 3; ++i) {
		if (memcmp(&mat1->IndirectTextureMatricies, &mat2->IndirectTextureMatricies,
                    sizeof (union Matrix2x4f)))
			return false;
	}

	for (int i = 0; i < 2; ++i) {
		if (memcmp(&mat1->ColorMatRegs, &mat2->ColorMatRegs, sizeof (union Vector4uc)))
			return false;
	}

	for (int i = 0; i < 2; ++i) {
		if (memcmp(&mat1->ColorAmbRegs, &mat2->ColorAmbRegs, sizeof (union Vector4uc)))
			return false;
	}

	for (int i = 0; i < 4; ++i) {
		if (memcmp(&mat1->ColorConstants, &mat2->ColorConstants, sizeof (union Vector4uc)))
			return false;
	}

	for (int i = 0; i < 4; ++i) {
		if (memcmp(&mat1->ColorRegisters, &mat2->ColorRegisters, sizeof (union Vector4s)))
			return false;
	}

	if (memcmp(mat1->FogInfo, mat2->FogInfo, sizeof (struct Fog)))
		return false;

	if (mat1->Culling != mat2->Culling)
		return false;

	for (int i = 0; i < 2; ++i) {
		if (memcmp(mat1->LightChannels[i]->Color, mat2->LightChannels[i]->Color,
		           sizeof (struct ColorChannelControl)))
			return false;

		if (memcmp(mat1->LightChannels[i]->Alpha, mat2->LightChannels[i]->Alpha,
		           sizeof (struct ColorChannelControl)))
			return false;
	}

	for (int i = 0; i < 8; ++i) {
		if (memcmp(mat1->TextureGenerators[i], mat2->TextureGenerators[i],
		           sizeof (struct TextureGenerator)))
			return false;
	}

	for (int i = 0; i < 16; ++i) {
		if (memcmp(mat1->TEVStages, mat2->TEVStages, sizeof (struct TextureEnvironmentStage)))
			return false;
	}

	for (int i = 0; i < 4; ++i) {
		if (memcmp(mat1->IndirectStages, mat2->IndirectStages, sizeof (struct IndirectTextureStage)))
			return false;
	}

	if (memcmp(mat1->AlphaTest, mat2->AlphaTest, sizeof (struct AlphaTest)))
		return false;

	if (memcmp(mat1->BlendInfo, mat2->BlendInfo, sizeof (struct Blend)))
		return false;

	if (memcmp(mat1->NBTScale, mat2->NBTScale, sizeof (struct NBT)))
		return false;

	return true;
}
