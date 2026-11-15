# Operating Systems Tutorial 5

A collection of standalone C exercises and sample text files for an operating-systems tutorial. Each `questionN.c` source file is an independent program.

## Requirements

- A C compiler such as Clang or GCC
- A POSIX-compatible shell

## Compile and run an exercise

From the repository root, compile one question at a time:

```bash
cc -std=c11 -Wall -Wextra -Wpedantic question1.c -o question1
./question1
```

Replace `question1` with the source file for the exercise you want to run. Some exercises read the accompanying text files, so run programs from the repository root.

## Contents

- `question1.c` through `question5.c`: tutorial exercise source
- `grades-1.txt`, `bellcurve.txt`: sample input data

Compiled executables in the repository are platform-specific; rebuild from the `.c` sources when needed.
