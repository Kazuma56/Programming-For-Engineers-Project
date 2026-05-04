#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
#include "io.h"

int main(void) {
    // 1. Hardcode the file name right here
    const char *filename = "power_quality_log.csv";

    WaveformSample *samples = NULL;

    // 2. Pass the hardcoded 'filename' instead of 'argv[1]'
    int n = load_csv(filename, &samples);

    if (n == 0) return EXIT_FAILURE;

    PhaseMetrics A = analyse_phase(samples, n, get_phase_A);
    PhaseMetrics B = analyse_phase(samples, n, get_phase_B);
    PhaseMetrics C = analyse_phase(samples, n, get_phase_C);
    SystemMetrics sys = analyse_system(samples, n, &A, &B, &C);

    // 3. Pass the hardcoded 'filename' to the reports
    print_report(filename, &A, &B, &C, &sys, n);
    write_report("results.txt", filename, &A, &B, &C, &sys, n);

    free(samples);
    return EXIT_SUCCESS;
}