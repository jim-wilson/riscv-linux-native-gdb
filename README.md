Work has moved into the FSF GDB source tree.  See
    http://sourceware.org/gdb
This tree is now obsolete, except for the info about linux kernel and glibc
patches.

At the time this was written, my branch requires 4 Linux Kernel patches.  A
patch file that includes all four of these patches is in riscv-linux.txt in
the top level dir. The first patch has been added into the official tree.  It
can be found at
    https://patchwork.kernel.org/patch/10458899
The second patch is in the queue to be applied soon.  It can be found at
    https://patchwork.kernel.org/patch/10524925/
The third patch is in the queue to be applied soon.  It can be found at
    https://github.com/riscv/riscv-linux/issues/140
and involves fixing arch/riscv/include/uapi/asm/syscalls.h to remove the
include guards.  The fourth patch is an initial attempt to add FP register
ptrace support.  This patch requires more work, on both the linux kernel and
gdb sides.

There is also a GLIBC patch required for thread debugging support.
    https://sourceware.org/ml/libc-alpha/2018-07/msg00303.html
