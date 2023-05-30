# LKM

Loadable kernel module for a priority-queue inside the kernel space with proc and ioctl interface. In addition to this, the module handle concurrency, mutual exclusion, memory management, process management, and io-control.

## Directory Structure

```
ioctl
    └── library
        └── ioctl_commands.h
        └── map.h
        └── pqueue.h
    └── test
        └── test.c
    └── A2_aos.c
    └── Makefile
    └── problem_statement.pdf

proc
    └── PartA
        └── .config-5.6.9
        └── report_G1.pdf
    └── PartB
        └── library
            └── map.h
            └── pqueue.h
        └── test
            └── test.c
        └── A1_partB.c
        └── Makefile
    └── problem_statement.pdf

README.md
```

# Contact Me

These are Assignments of CS60038: Advances in OS design course in IIT Kharagpur, taught by [Dr. Sandip Chakraborty](https://cse.iitkgp.ac.in/~sandipc/). For questions and general feedback, contact [Prasenjit Karmakar](https://www.linkedin.com/in/prasenjit52282).