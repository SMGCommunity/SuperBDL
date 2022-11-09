#ifndef __BMD_H
#define __BMD_H

#include <stdbool.h>

struct model_data {
	int placeholder;
};

bool write_bmd(struct model_data parsed_model_data, FILE *output_fp, bool write_bdl, bool sm3das);

#endif /* __BMD_H */
