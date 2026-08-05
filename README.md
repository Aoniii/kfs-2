# KFS-1 — Kernel From Scratch

*Read this in [French](README.fr.md).*

A minimal **32-bit (i386) kernel** that boots via **GRUB**, written from scratch
with no host library. This is the first project of the 42 *Kernel From Scratch*
series: boot, screen output, and a tiny freestanding library.

---

## What it does

Boots through GRUB (Multiboot), sets up a stack, jumps into `kernel_main`, and
prints to the VGA text buffer.

**Mandatory**
- Bootable via GRUB (Multiboot header)
- ASM boot entry + custom linker script
- Minimal freestanding library (`strlen`, `strcmp`, `printk`, …)
- Writes characters to the screen — displays `42`

**Bonus**
- Scrolling and hardware cursor
- Colors
- `printk` (`%d %s %x %c %p`)
- PS/2 keyboard input
- 3 virtual TTYs, switchable with **F1 / F2 / F3**

---

## Requirements

The kernel must be built with an **i686-elf cross-compiler** (it cannot be
linked against the host libc). You also need GRUB tooling and an emulator.

Two scripts handle the setup:

```sh
sudo ./scripts/install-deps.sh   # host packages (gcc, grub, xorriso, qemu, ...)
./scripts/build-i686-elf.sh      # build i686-elf-gcc into $HOME/opt/cross
```

`install-deps.sh` supports apt / dnf / pacman. `build-i686-elf.sh` must be run
as a normal user (it installs into a user prefix).

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

Inside QEMU: type on the keyboard to write, press **F1/F2/F3** to switch TTYs.

---

## Project structure

```
.
├── Makefile              # thin top-level: wires everything, high-level targets
├── mk/                   # Makefile fragments
│   ├── config.mk         #   toolchain, paths, compiler/linker flags
│   ├── rules.mk          #   build/link/iso rules + auto header deps
│   └── toolchain.mk      #   cross-compiler sanity check
├── include/              # public headers (one per module)
│   ├── types.h  io.h  str.h  put.h  fmt.h
│   ├── vga.h  cursor.h  keyboard.h
│   └── kernel.h          #   umbrella header + compiler guards
├── src/
│   ├── boot/             # boot.s (Multiboot + _start) + linker.ld
│   ├── kernel/           # kernel_main
│   ├── lib/              # freestanding mini-libc, one function per file
│   │   ├── str/          #   strlen, strcmp
│   │   ├── put/          #   putchar, putstr, putnbr, puthex, putptr
│   │   └── fmt/          #   printk
│   └── drivers/
│       ├── vga/          #   text terminal + TTYs (+ private vga_internal.h)
│       ├── cursor/       #   hardware VGA cursor
│       └── keyboard/     #   PS/2 keyboard (polling)
├── grub/grub.cfg         # GRUB menu entry
└── scripts/              # dependency + cross-compiler setup
```

Each `src/**` directory carries a `module.mk` that lists its sources
explicitly (no wildcards). Adding a module = create its `module.mk` and add one
`include` line to the top-level `Makefile`.

---

## How it boots

1. **GRUB** finds the **Multiboot header** (magic `0x1BADB002`) that `boot.s`
   places in the first bytes of the binary, and loads the kernel at **2 MiB**
   (`linker.ld`).
2. **`_start`** (in `boot.s`) sets up a 16 KiB stack, then `call kernel_main`.
3. **`kernel_main`** initializes the terminal and prints to the VGA text buffer
   at `0xB8000`, then loops polling the keyboard.

The linker script (`src/boot/linker.ld`) defines the entry point and places
`.multiboot` + `.text` first so GRUB recognizes the image.

---

## Notes

- **Architecture:** i386 (x86, 32-bit) — mandatory for the series.
- **Freestanding:** compiled with `-ffreestanding -nostdlib -nodefaultlibs`;
  no host libc, so the kernel provides its own `strlen`, `printk`, etc.
- **The image must stay under 10 MB** (subject requirement) — the ISO is ~4 MB.
