#include "mat3.h"

bool readMAT3(FILE* fp, struct J3DMaterial* outputArray, unsigned int* elementCount)
{
    return false;
}

bool matcmp(struct J3DMaterial* mat1, struct J3DMaterial* mat2) {
    if (strcmp(mat1->Name, mat2->Name))
        return false;

    if (mat1->MaterialMode != mat2->MaterialMode)
        return false;

    if (mat1->EnableDithering != mat2->EnableDithering)
        return false;

    return true;
}
