/* Native-dependent code for GNU/Linux RISC-V.
   Copyright (C) 2018 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "defs.h"
#include "regcache.h"
#include "gregset.h"
#include "linux-nat.h"
#include "elf.h"
#include "riscv-tdep.h"

#include <sys/ptrace.h>

class riscv_linux_nat_target final : public linux_nat_target
{
public:
  /* Add our register access methods.  */
  void fetch_registers (struct regcache *regcache, int regnum) override;
  void store_registers (struct regcache *regcache, int regnum) override;
};

static riscv_linux_nat_target the_riscv_linux_nat_target;

void
supply_gregset (struct regcache *regcache, const prgregset_t *gregs)
{
  int i;
  const elf_greg_t *regp = *gregs;

  for (i = 1; i < 32; i++)
    regcache->raw_supply (i, regp + i);

  /* GDB stores PC in reg 32.  Linux kernel stores it in reg 0.  */
  regcache->raw_supply (32, regp + 0);

  /* Fill the inaccessible zero register with zero.  */
  regcache->raw_supply_zeroed (0);
}

void
supply_fpregset (struct regcache *regcache, const prfpregset_t *fpregs)
{
  abort ();
}

void
fill_gregset (const struct regcache *regcache, prgregset_t *gregs, int regnum)
{
  struct gdbarch *gdbarch = regcache->arch ();
  elf_greg_t *regp = *gregs;

  if (regnum == -1 || 1)
    {
      for (int i = 1; i < 32; i++)
	regcache->raw_collect (i, regp + i);

      regcache->raw_collect (32, regp);
    }
}

void
fill_fpregset (const struct regcache *regcache,
	       prfpregset_t *fpregs, int regnum)
{
  abort ();
}

/* Fetch REGNUM (or all registers if REGNUM == -1) from the target
   using any working method.  */

void
riscv_linux_nat_target::fetch_registers (struct regcache *regcache, int regnum)
{
  struct gdbarch *gdbarch = regcache->arch ();
  int tid;

  tid = get_ptrace_pid (regcache->ptid());

  if (regnum == -1 || (regnum >= 0 && regnum <= 32))
    {
      struct iovec iov;
      elf_gregset_t regs;

      iov.iov_base = &regs;
      iov.iov_len = sizeof (regs);

      if (ptrace (PTRACE_GETREGSET, tid, NT_PRSTATUS,
		  (PTRACE_TYPE_ARG3) &iov) == -1)
	perror_with_name (_("Couldn't get registers"));
      else
	supply_gregset (regcache, &regs);
    }
  else if (regnum >= 33 && regnum <= 64)
    {
      printf ("unsupported fp register read\n");
    }
  else if (regnum == RISCV_CSR_MISA_REGNUM)
    {
      /* ??? Need to add a ptrace call for this.  */
      regcache->raw_supply_zeroed (regnum);
    }
  else if (regnum >= RISCV_FIRST_CSR_REGNUM && regnum <= RISCV_LAST_CSR_REGNUM)
    {
      printf ("unsupported csr read\n");
    }
  else
    {
      printf ("unknown register read\n");
    }
}

/* Store REGNUM (or all registers if REGNUM == -1) to the target
   using any working method.  */

void
riscv_linux_nat_target::store_registers (struct regcache *regcache, int regnum)
{
  struct gdbarch *gdbarch = regcache->arch ();
  int tid;

  elf_greg_t reg;
  regcache->raw_collect (regnum, &reg);

  tid = get_ptrace_pid (regcache->ptid ());

  if (regnum == -1 || 1)
    {
      struct iovec iov;
      elf_gregset_t regs;

      iov.iov_base = &regs;
      iov.iov_len = sizeof (regs);

      if (ptrace (PTRACE_GETREGSET, tid, NT_PRSTATUS,
		  (PTRACE_TYPE_ARG3) &iov) == -1)
	perror_with_name (_("Couldn't get registers"));
      else
	{
	  fill_gregset (regcache, &regs, regnum);

	  if (ptrace (PTRACE_SETREGSET, tid, NT_PRSTATUS,
		      (PTRACE_TYPE_ARG3) &iov) == -1)
	    perror_with_name (_("Couldn't set registers"));
	}
    }
}

void
_initialize_riscv_linux_nat (void)
{
  /* Register the target.  */
  linux_target = &the_riscv_linux_nat_target;
  add_inf_child_target (&the_riscv_linux_nat_target);
}
