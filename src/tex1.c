#include "tex1.h"

bool texcmp(struct JUTTexture* tex1, struct JUTTexture* tex2) {
	if (strcmp(tex1->Name, tex2->Name))
		return false;

	if (tex1->AlphaSetting != tex2->AlphaSetting)
		return false;

	if (tex1->ClampLODBias != tex2->ClampLODBias)
		return false;

	if (tex1->EnableMipmaps != tex2->EnableMipmaps)
		return false;

	if (tex1->MaxAnisotropy != tex2->MaxAnisotropy)
		return false;

	if (tex1->Format != tex2->Format)
		return false;

	if (tex1->PaletteFormat != tex2->PaletteFormat)
		return false;

	if (tex1->WrapS != tex2->WrapS)
		return false;

	if (tex1->WrapT != tex2->WrapT)
		return false;

	if (tex1->MagnificationFilter != tex2->MagnificationFilter)
		return false;

	if (tex1->MinificationFilter != tex2->MinificationFilter)
		return false;

	if (tex1->MinLOD != tex2->MinLOD)
		return false;

	if (tex1->MaxLOD != tex2->MaxLOD)
		return false;

	if (tex1->LODBias != tex2->LODBias)
		return false;

	if (tex1->EnableEdgeLOD != tex2->EnableEdgeLOD)
		return false;

	if (tex1->Width != tex2->Width)
		return false;

	if (tex1->Height != tex2->Height)
		return false;

	if (tex1->ImageCount != tex2->ImageCount)
		return false;

#if 0
	if (!memcmp(tex1->ImageData, tex2->ImageData, 0))
		return false;

	if (!memcmp(tex1->PaletteData, tex2->PaletteData, 0))
		return false;
#endif

	return true;
}
