# Operating Systems Assignments

Coursework repository for operating systems assignments covering assembly,
basic shell utilities, scheduling, kernel programming, synchronization,
inter-process communication, and process inspection.

## Repository Layout

- `Assignment0/` contains an assembly assignment, Makefile, generated object and
  listing files, and assignment brief.
- `Assignment1/` contains a simple shell and basic command implementations in C
  such as `cat`, `cd`, `date`, `echo`, `ls`, `mkdir`, `pwd`, and `rm`.
- `Assignment2/` contains scheduling experiments and kernel-memory-copy work,
  including Makefiles, scripts, plots, and README PDFs.
- `Assignment3/` contains synchronization, IPC, and process-inspection work:
  - dining-philosophers variants using semaphores and ordering strategies,
  - FIFO, shared-memory, and socket sender/receiver programs,
  - a process module assignment.

## Requirements

Use a Unix-like environment with GCC and Make. Kernel-related assignments may
require a Linux environment configured according to the assignment brief.

## Build

Most implementation directories include a `Makefile`. Enter the relevant
assignment subdirectory and run:

```sh
make
```

## Usage

Read the assignment PDF or README PDF in the relevant directory before running
the programs. Some assignments generate binaries or plots in place.

## Notes

Compiled binaries and generated artifacts are included for some assignments, so
source files and reports should be treated as the primary reference material.
