#ifndef IO_H
#define IO_H
#include "waveform.h"

int  load_csv(const char *filename, WaveformSample **out);
void print_report(const char *source, const PhaseMetrics *A, const PhaseMetrics *B, const PhaseMetrics *C, const SystemMetrics *sys, int n);
void write_report(const char *filename, const char *source, const PhaseMetrics *A, const PhaseMetrics *B, const PhaseMetrics *C, const SystemMetrics *sys, int n);

#endif