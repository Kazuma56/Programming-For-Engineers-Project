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

static double compute_std(const double *v, int n, double mean, double *variance) {
    double sum = 0.0;
    const double *p = v;
    for (int i = 0; i < n; i++, p++) { double d = *p - mean; sum += d * d; }
    *variance = sum / n;
    return sqrt(*variance);
}

static int count_clips(const double *v, int n) {
    int c = 0;
    const double *p = v;
    for (int i = 0; i < n; i++, p++) { double a = *p >= 0 ? *p : -(*p); if (a >= 324.9) c++; }
    return c;
}

static int check_tolerance(double rms) {
    return (rms >= 207.0 && rms <= 253.0) ? 1 : 0;
}