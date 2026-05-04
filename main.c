#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
#include "io.h"

int main(int argc, char *argv[]) {
    if (argc != 2) { fprintf(stderr, "Usage: %s <csv_file>\n", argv[0]); return EXIT_FAILURE; }
    WaveformSample *samples = NULL;
    int n = load_csv(argv[1], &samples);
    if (n == 0) return EXIT_FAILURE;
    PhaseMetrics A = analyse_phase(samples, n, get_phase_A);
    PhaseMetrics B = analyse_phase(samples, n, get_phase_B);
    PhaseMetrics C = analyse_phase(samples, n, get_phase_C);
    SystemMetrics sys = analyse_system(samples, n, &A, &B, &C);
    print_report(argv[1], &A, &B, &C, &sys, n);
    write_report("results.txt", argv[1], &A, &B, &C, &sys, n);
    free(samples); return EXIT_SUCCESS;
}

