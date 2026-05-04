#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
#include "io.h"

int main(int argc, char *argv[]) {
    if (argc != 2) { fprintf(stderr, "Usage: %s <csv_file>\n", argv[0]); return EXIT_FAILURE; }

