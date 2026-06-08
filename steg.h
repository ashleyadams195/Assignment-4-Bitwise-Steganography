#ifndef STEG_H
#define STEG_H
#include "ppm.h"

void encode(const char *input_ppm, const char *payload, const char *output_ppm);
void decode(const char *input_ppm, const char *output_file);

#endif
