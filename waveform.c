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

PhaseMetrics analyse_phase(const WaveformSample *samples, int n, PhaseSelector selector) {
    PhaseMetrics m;
    double *voltages = (double *)malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) voltages[i] = selector(&samples[i]);
    m.rms_voltage   = compute_rms(voltages, n);
    m.peak_to_peak  = compute_peak_to_peak(voltages, n);
    m.dc_offset     = compute_mean(voltages, n);
    m.std_deviation = compute_std(voltages, n, m.dc_offset, &m.variance);
    m.clip_count    = count_clips(voltages, n);
    m.in_tolerance  = check_tolerance(m.rms_voltage);
    m.health_flags  = 0;
    if (m.clip_count > 0) m.health_flags |= 0x01;
    if (!m.in_tolerance)  m.health_flags |= 0x02;
    free(voltages);
    return m;
}

SystemMetrics analyse_system(const WaveformSample *samples, int n, const PhaseMetrics *A, const PhaseMetrics *B, const PhaseMetrics *C) {
    SystemMetrics sys;
    sys.avg_rms          = (A->rms_voltage + B->rms_voltage + C->rms_voltage) / 3.0;
    sys.total_clips      = A->clip_count + B->clip_count + C->clip_count;
    sys.all_in_tolerance = A->in_tolerance && B->in_tolerance && C->in_tolerance;
    double freq_sum = 0.0, pf_sum = 0.0, thd_sum = 0.0;
    const WaveformSample *p = samples;
    for (int i = 0; i < n; i++, p++) { freq_sum += p->frequency; pf_sum += p->power_factor; thd_sum += p->thd_percent; }
    sys.avg_frequency    = freq_sum / n;
    sys.avg_power_factor = pf_sum  / n;
    sys.avg_thd          = thd_sum / n;
    return sys;
}