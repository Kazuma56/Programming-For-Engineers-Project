#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <stdint.h>

typedef struct {
    double timestamp;
    double phase_A_voltage;
    double phase_B_voltage;
    double phase_C_voltage;
    double line_current;
    double frequency;
    double power_factor;
    double thd_percent;
} WaveformSample;

typedef struct {
    char   name[8];
    double rms_voltage;
    double peak_to_peak;
    double dc_offset;
    double std_deviation;
    double variance;
    int    clip_count;
    int    in_tolerance;
    uint8_t health_flags;
} PhaseMetrics;

typedef struct {
    double avg_rms;
    double avg_frequency;
    double avg_power_factor;
    double avg_thd;
    int    all_in_tolerance;
    int    total_clips;
} SystemMetrics;

/* Phase selector function pointer type */
typedef double (*PhaseSelector)(const WaveformSample *);

/* Phase selector functions */
double get_phase_A(const WaveformSample *s);
double get_phase_B(const WaveformSample *s);
double get_phase_C(const WaveformSample *s);

/* Analysis functions */
PhaseMetrics  analyse_phase(const WaveformSample *samples, int n, PhaseSelector selector);
SystemMetrics analyse_system(const WaveformSample *samples, int n, const PhaseMetrics *A, const PhaseMetrics *B, const PhaseMetrics *C);

#endif /* WAVEFORM_H */