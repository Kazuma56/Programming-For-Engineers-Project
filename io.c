#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE 512

int load_csv(const char *filename, WaveformSample **out) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { fprintf(stderr, "Error: could not open '%s'\n", filename); return 0; }
    char line[MAX_LINE];
    fgets(line, MAX_LINE, fp);
    int count = 0;
    while (fgets(line, MAX_LINE, fp)) if (line[0] != '\n') count++;
    if (count == 0) { fclose(fp); return 0; }
    *out = (WaveformSample *)malloc(count * sizeof(WaveformSample));
    rewind(fp); fgets(line, MAX_LINE, fp);
    int idx = 0;
    WaveformSample *p = *out;
    while (fgets(line, MAX_LINE, fp) && idx < count) {
        if (line[0] == '\n') continue;
        if (sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
            &p->timestamp, &p->phase_A_voltage, &p->phase_B_voltage,
            &p->phase_C_voltage, &p->line_current, &p->frequency,
            &p->power_factor, &p->thd_percent) == 8) { p++; idx++; }
    }
    fclose(fp);
    return idx;
}

static void print_phase(FILE *fp, const char *label, const PhaseMetrics *m) {
    fprintf(fp, "\n--- %s ---\n", label);
    fprintf(fp, "  RMS Voltage    : %.4f V\n",  m->rms_voltage);
    fprintf(fp, "  Peak-to-Peak   : %.4f V\n",  m->peak_to_peak);
    fprintf(fp, "  DC Offset      : %.6f V\n",  m->dc_offset);
    fprintf(fp, "  Std Deviation  : %.4f V\n",  m->std_deviation);
    fprintf(fp, "  Variance       : %.4f V^2\n",m->variance);
    fprintf(fp, "  Clipped Samples: %d\n",      m->clip_count);
    fprintf(fp, "  Tolerance      : %s\n",      m->in_tolerance ? "PASS" : "FAIL");
    fprintf(fp, "  Health Flags   : 0x%02X\n",  m->health_flags);
}

static void print_system(FILE *fp, const SystemMetrics *sys) {
    fprintf(fp, "\n--- System ---\n");
    fprintf(fp, "  Avg RMS        : %.4f V\n",  sys->avg_rms);
    fprintf(fp, "  Avg Frequency  : %.4f Hz\n", sys->avg_frequency);
    fprintf(fp, "  Avg Power Factor: %.4f\n",   sys->avg_power_factor);
    fprintf(fp, "  Avg THD        : %.4f %%\n", sys->avg_thd);
    fprintf(fp, "  Total Clips    : %d\n",      sys->total_clips);
    fprintf(fp, "  All In Tolerance: %s\n",     sys->all_in_tolerance ? "YES" : "NO");
    fprintf(fp, "  Status         : %s\n",      (sys->all_in_tolerance && sys->total_clips == 0) ? "COMPLIANT" : "NON-COMPLIANT");
}

void print_report(const char *source, const PhaseMetrics *A, const PhaseMetrics *B, const PhaseMetrics *C, const SystemMetrics *sys, int n) {
    printf("\n==============================\n  POWER QUALITY RESULTS\n==============================\n");
    printf("  Source: %s | Samples: %d\n", source, n);
    print_phase(stdout, "Phase A", A);
    print_phase(stdout, "Phase B", B);
    print_phase(stdout, "Phase C", C);
    print_system(stdout, sys);
    printf("\n==============================\n");
}

void write_report(const char *filename, const char *source, const PhaseMetrics *A, const PhaseMetrics *B, const PhaseMetrics *C, const SystemMetrics *sys, int n) {
    FILE *fp = fopen(filename, "w");
    if (!fp) { fprintf(stderr, "Error: could not write '%s'\n", filename); return; }
    time_t now = time(NULL); char tb[64];
    strftime(tb, sizeof(tb), "%Y-%m-%d %H:%M:%S", localtime(&now));
    fprintf(fp, "==============================\n  POWER QUALITY REPORT\n==============================\n");
    fprintf(fp, "  Source: %s | Samples: %d | Generated: %s\n", source, n, tb);
    print_phase(fp, "Phase A", A);
    print_phase(fp, "Phase B", B);
    print_phase(fp, "Phase C", C);
    print_system(fp, sys);
    fprintf(fp, "\n==============================\n");
    fclose(fp);
    printf("Report written to '%s'\n", filename);
}