# Power Quality Waveform Analyser

A command-line C application that reads a 3-phase power quality log file, computes key waveform metrics for each phase, and writes a structured report.

Built for UGMFGT-15-1 Programming for Engineers | UWE Bristol | 2025–26

---

## GitHub Repository

https://github.com/YOUR_USERNAME/Programming-For-Engineers-Project

> Replace the link above with your actual GitHub repository URL before submitting.

---

## What the Program Does

- Loads a CSV file of 1,000 power quality samples (3-phase, 50 Hz)
- Computes per-phase: RMS voltage, peak-to-peak amplitude, DC offset, standard deviation, variance, clipping count, and tolerance compliance
- Encodes phase health status as a bitwise flag (`uint8_t`)
- Computes system-wide averages: RMS, frequency, power factor, THD
- Prints results to the console
- Writes a full report to `results.txt`

---

## File Structure

```
Programming-For-Engineers-Project/
├── main.c          # Entry point — argument handling and function orchestration
├── waveform.c      # All analysis functions (RMS, peak-to-peak, DC offset, etc.)
├── waveform.h      # Struct definitions and function declarations
├── io.c            # CSV loading and report output
├── io.h            # I/O function declarations
├── CMakeLists.txt  # CMake build configuration
├── README.md       # This file
├── logbook.docx    # Development logbook
└── power_quality_log.csv  # Dataset (1,000 samples)
```

---

## How to Build and Run

### Option 1 — CLion (Recommended)

1. Open CLion and select **File → Open**, then navigate to the project folder.
2. CLion will detect `CMakeLists.txt` automatically and configure the project.
3. Go to **Run → Edit Configurations**.
4. Under **CMake Application**, find `Programming_For_Engineers_Project`.
5. In the **Program arguments** field, enter the path to the CSV file (use quotes if your path contains spaces):
   ```
   "C:\path\to\power_quality_log.csv"
   ```
6. Click **OK**, then press **Ctrl+F9** to build and the green **Run** button to execute.

### Option 2 — Command Line (gcc)

From inside the project folder, compile with:

```bash
gcc -std=c99 -Wall -Wextra -Wpedantic -o power_quality_analyser main.c waveform.c io.c -lm
```

Then run with:

```bash
./power_quality_analyser power_quality_log.csv
```

On Windows (MinGW):

```bash
gcc -std=c99 -Wall -Wextra -Wpedantic -o power_quality_analyser.exe main.c waveform.c io.c -lm
power_quality_analyser.exe power_quality_log.csv
```

### Option 3 — CMake (Command Line)

```bash
mkdir build && cd build
cmake ..
cmake --build .
./Programming_For_Engineers_Project ../power_quality_log.csv
```

---

## Expected Output

When run successfully, the program prints to the console and writes `results.txt`:

```
==============================
  POWER QUALITY RESULTS
==============================
  Source: power_quality_log.csv | Samples: 1000

--- Phase A ---
  RMS Voltage    : 230.9352 V
  Peak-to-Peak   : 651.4883 V
  DC Offset      : 1.307860 V
  Std Deviation  : 230.9315 V
  Variance       : 53329.8600 V^2
  Clipped Samples: 29
  Tolerance      : PASS
  Health Flags   : 0x01

--- Phase B ---
  ...

--- System ---
  Avg RMS        : 230.3395 V
  Status         : NON-COMPLIANT

==============================
Report written to 'results.txt'
```

`results.txt` is written to the working directory (the folder the program runs from).

---

## Error Handling

| Scenario | Behaviour |
|---|---|
| No filename provided | Prints usage message and exits |
| File not found | Prints error to stderr and exits |
| Empty CSV file | Prints error and exits cleanly |
| Memory allocation failure | Program exits without crashing |

---

## Build Requirements

- C99-compliant compiler (gcc, MinGW, or Clang)
- CMake 3.15 or later (for CLion builds)
- Math library (`-lm`) — required for `sqrt()` in RMS and standard deviation calculations
