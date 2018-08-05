Work in progress repo for RISC-V native linux support.  May be broken at any
time.

I like to use rebase to keep my patches at the top of the tree for easy
viewing.  Use "git pull --rebase" to update.

The master branch tracks FSF binutils top of tree.  The jimw-riscv-linux-gdb
branch (the default branch) is master plus my patches.  So my patches can be
viewed with "git diff master jimw-riscv-linux-gdb".

At the time this was written, my branch requires 4 Linux Kernel patches.  A
patch file that includes all four of these patches is in riscv-linux.txt in
the top level dir. The first patch is submitted and should be added soon.  It
can be found at
    https://patchwork.kernel.org/patch/10458899
The second patch is submitted and waiting review, but has already been
discussed and conditionally approved.  It can be found at
    https://patchwork.kernel.org/patch/10524925/
The third patch is mentioned in
    https://github.com/riscv/riscv-linux/issues/140
and involves fixing arch/riscv/include/uapi/asm/syscalls.h to remove the
include guards.  The fourth patch is an initial attempt to add FP register
ptrace support.  This patch requires more work, on both the linux kernel and
gdb sides.

There is also a GLIBC patch required for thread debugging support.
    https://sourceware.org/ml/libc-alpha/2018-07/msg00303.html
