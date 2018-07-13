/* Target-dependent code for GNU/Linux on RISC-V processors.
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
#include "riscv-tdep.h"
#include "osabi.h"
#include "glibc-tdep.h"
#include "linux-tdep.h"
#include "solib-svr4.h"

static void
riscv_linux_init_abi (struct gdbarch_info info, struct gdbarch *gdbarch)
{
  linux_init_abi (info, gdbarch);

  set_gdbarch_software_single_step (gdbarch, riscv_software_single_step);

  set_solib_svr4_fetch_link_map_offsets (gdbarch,
					 (riscv_isa_xlen (gdbarch) == 4
					  ? svr4_ilp32_fetch_link_map_offsets
					  : svr4_lp64_fetch_link_map_offsets));

  /* GNU/Linux uses SVR4-style shared libraries.  */
  set_gdbarch_skip_trampoline_code (gdbarch, find_solib_trampoline_target);

  /* GNU/Linux uses the dynamic linker included in the GNU C Library.  */
  set_gdbarch_skip_solib_resolver (gdbarch, glibc_skip_solib_resolver);

  /* Enable TLS support.  */
  set_gdbarch_fetch_tls_load_module_address (gdbarch,
                                             svr4_fetch_objfile_link_map);
}

void
_initialize_riscv_linux_tdep (void)
{
  gdbarch_register_osabi (bfd_arch_riscv, 0, GDB_OSABI_LINUX,
			  riscv_linux_init_abi);
}
