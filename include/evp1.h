#ifndef __EVP1_H
#define __EVP1_H

#include <stdbool.h>
#include <bmd.h>

#define MAGIC_EVP1 "EVP1"

bool writeEVP1(const struct aiScene *data);

#endif /* __EVP1_H */