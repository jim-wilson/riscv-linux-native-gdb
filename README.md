Work in progress repo for RISC-V native linux support.  May be broken at any
time.

The master branch tracks FSF binutils top of tree.  The jimw-riscv-linux-gdb
branch (the default branch) is master plus my patches.  So my patches can be
viewed with "git diff master jimw-riscv-linux-gdb".

At the time this was written, my branch requires 4 Linux Kernel patches.  A
patch file that includes all four of these patches is in riscv-linux.txt in
the top level dir. The first patch is submitted and should be added soon.  It
can be found at
    https://patchwork.kernel.org/patch/10458899
The second patch is not submitted yet, waiting for discussion with other gdb
developers.  This patch deletes the line that adds 0x4 to sepc in do_trap_break
in arch/riscv/kernel/traps.c.  The third patch is mentioned in
    https://github.com/riscv/riscv-linux/issues/140
and involved fixing arch/riscv/include/uapi/asm/syscalls.h to remove the
include guards.  The fourth patch is an initial attempt to add FP register
ptrace support.  This patch requires more work.  

There is also a GLIBC patch required for thread debugging support.
    https://sourceware.org/ml/libc-alpha/2018-07/msg00303.html
