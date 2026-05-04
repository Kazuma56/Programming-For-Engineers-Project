#include "waveform.h"
#include <math.h>
#include <stdlib.h>

double get_phase_A(const WaveformSample *s) { return s->phase_A_voltage; }
double get_phase_B(const WaveformSample *s) { return s->phase_B_voltage; }
double get_phase_C(const WaveformSample *s) { return s->phase_C_voltage; }

static double compute_rms(const double *v, int n) {
    double sum = 0.0;
    const double *p = v;
    for (int i = 0; i < n; i++, p++) sum += (*p) * (*p);
    return sqrt(sum / n);
}

static double compute_mean(const double *v, int n) {
    double sum = 0.0;
    const double *p = v;
    for (int i = 0; i < n; i++, p++) sum += *p;
    return sum / n;
}

static double compute_peak_to_peak(const double *v, int n) {
    double mn = *v, mx = *v;
    const double *p = v + 1;
    for (int i = 1; i < n; i++, p++) { if (*p < mn) mn = *p; if (*p > mx) mx = *p; }
    return mx - mn;
}