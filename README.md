# KFS-2 — Kernel From Scratch

*Read this in [French](README.fr.md).*

A 32-bit (i386) kernel that boots via GRUB, extended with a **Global Descriptor
Table (GDT)** and a **kernel stack dump**. This is the second project of the 42
*Kernel From Scratch* series — it builds directly on KFS-1 (boot, VGA, freestanding
library) and adds memory segmentation.

---

## What it does

Sets up its own GDT, loads it into the CPU, then runs a small interactive shell.

**Mandatory**
- A **GDT** with 7 entries: null + kernel code/data/stack + user code/data/stack
- The GDT is placed at the fixed address **`0x00000800`** and loaded with `lgdt`
- A **stack dump** tool that prints the kernel stack in a `hexdump`-style layout

**Bonus**
- A minimal in-kernel **shell** with a few debug commands

Everything from KFS-1 still works: GRUB boot, VGA text output, scrolling, hardware
cursor, colors, `printk`, PS/2 keyboard, and 3 TTYs (**F1 / F2 / F3**).

---

## The shell

Each TTY has its own prompt (`kfs>`) and its own input line.

| Command  | Effect                                             |
|----------|----------------------------------------------------|
| `stack`  | Hexdump-style dump of the kernel stack around `esp`|
| `gdt`    | Print the GDTR (base/limit) and segment registers  |
| `regs`   | Dump the general-purpose registers                 |
| `clear`  | Clear the current screen                           |
| `reboot` | Reboot the machine (8042 CPU reset)                |
| `halt`   | Stop the CPU (`cli` + `hlt`)                        |

---

## Requirements

The kernel must be built with an **i686-elf cross-compiler** (it cannot be linked
against the host libc). You also need GRUB tooling and an emulator.

```sh
sudo ./scripts/install-deps.sh   # host packages (gcc, grub, xorriso, qemu, ...)
./scripts/build-i686-elf.sh      # build i686-elf-gcc into $HOME/opt/cross
```

`install-deps.sh` supports apt / dnf / pacman. `build-i686-elf.sh` runs as a
normal user (installs into a user prefix).

---

## Build & run

```sh
make        # build kfs.iso (default)
make run    # boot the ISO in QEMU
```

| Target   | Description                                   |
|----------|-----------------------------------------------|
| `make` / `make all` | Build the bootable ISO (`kfs.iso`)     |
| `make kernel`       | Build only the kernel binary (`kfs.bin`) |
| `make iso`          | Build the GRUB-bootable ISO            |
| `make run`          | Boot the ISO in `qemu-system-i386`     |
| `make clean`        | Remove build objects and ISO staging   |
| `make fclean`       | `clean` + remove `kfs.bin` / `kfs.iso` |
| `make re`           | Full rebuild                           |

---

## Project structure

```
.
├── Makefile / mk/          # thin top-level + config/rules/toolchain fragments
├── include/                # public headers (one per module)
│   └── gdt.h  debug.h  shell.h  vga.h  keyboard.h  ...
├── src/
│   ├── boot/               # boot.s (Multiboot + _start) + linker.ld
│   ├── kernel/             # kernel_main
│   ├── gdt/                # gdt.c (build at 0x800) + gdt_flush.s (lgdt + reload)
│   ├── debug/              # stack_dump, gdt_debug, regs
│   ├── shell/              # shell.c, dispatch.c, cmd/ (reboot, halt)
│   ├── lib/                # freestanding mini-libc (str/ put/ fmt/)
│   └── drivers/            # vga/ (+ cursor/ keyboard/)
├── grub/grub.cfg
└── scripts/
```

Each `src/**` directory carries a `module.mk` listing its sources explicitly
(no wildcards); one `include` line in the top-level `Makefile` wires it in.

---

## How it boots

1. **GRUB** finds the **Multiboot header** in `boot.s` and loads the kernel at 2 MiB.
2. **`_start`** sets up a stack and calls `kernel_main`.
3. **`kernel_main`** runs **`gdt_init()` first** (build the GDT at `0x800`, load it
   with `lgdt`, reload the segment registers via a far jump), then initializes the
   terminal and starts the shell.

---

## The GDT, in short

The GDT is a table the CPU reads to translate a **segment selector** (held in
`CS`, `DS`, `SS`, …) into a memory region's **base, limit, and access rights**
(kernel vs user, code vs data). We use a **flat model**: every segment has base 0
and a 4 GiB limit, so segmentation is transparent — the GDT mainly defines the
ring 0 / ring 3 privilege boundary. Real per-region memory separation comes later
with paging (KFS-3).

### Verifying the GDT

The most reliable check is QEMU's monitor. In the QEMU window, press
**Ctrl+Alt+2** (or launch with `-monitor stdio`) and run:

```
info registers
```

You should see the GDT loaded at our address:

```
CS =0008 00000000 ffffffff 00cf9a00 DPL=0 CS32 [-R-]
DS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
GDT=     00000800 00000037
```

- `GDT= 00000800 00000037` → base `0x800`, limit 55 (7 entries)
- `CS=0008`, `DS=0010` → segment registers reloaded onto our descriptors
- `CPL=0` → running in ring 0

The kernel's own `gdt` command prints the same information from inside the kernel.

---

## Notes

- **Architecture:** i386 (x86, 32-bit) — mandatory for the series.
- **Freestanding:** compiled with `-ffreestanding -nostdlib -nodefaultlibs`; no
  host libc, so the kernel provides its own `strlen`, `printk`, etc.
- **The image must stay under 10 MB** (subject requirement).
