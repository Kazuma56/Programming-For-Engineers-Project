#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
#include "io.h"

int main(int argc, char *argv[]) {
    // 1. Check if the user actually typed the filename in the terminal
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <csv_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    WaveformSample *samples = NULL;

    // 2. Load the input file using the terminal argument (argv[1])
    int n = load_csv(argv[1], &samples);
    if (n == 0) return EXIT_FAILURE;

    // 3. Process the data
    PhaseMetrics A = analyse_phase(samples, n, get_phase_A);
    PhaseMetrics B = analyse_phase(samples, n, get_phase_B);
    PhaseMetrics C = analyse_phase(samples, n, get_phase_C);
    SystemMetrics sys = analyse_system(samples, n, &A, &B, &C);

    // 4. Print to console, and write the file to "results.txt"
    print_report(argv[1], &A, &B, &C, &sys, n);
    write_report("results.txt", argv[1], &A, &B, &C, &sys, n);

    // 5. Clean up memory
    free(samples);
    return EXIT_SUCCESS;
}