#ifndef IO_H
#define IO_H
#include "waveform.h"

int load_csv(const char *filename, WaveformSample **out);

#endif