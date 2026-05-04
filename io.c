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
    fgets(line, MAX_LINE, fp); /* skip header */
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
