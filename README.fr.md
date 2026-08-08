# KFS-2 — Kernel From Scratch

*Lire en [anglais](README.md).*

Un kernel 32 bits (i386) qui démarre via GRUB, enrichi d'une **Global Descriptor
Table (GDT)** et d'un **dump de la pile kernel**. C'est le deuxième projet de la
série 42 *Kernel From Scratch* — il repart directement de KFS-1 (boot, VGA,
bibliothèque freestanding) et ajoute la segmentation mémoire.

---

## Ce que ça fait

Construit sa propre GDT, la charge dans le CPU, puis lance un petit shell interactif.

**Mandatory**
- Une **GDT** de 7 entrées : null + kernel code/data/stack + user code/data/stack
- La GDT est placée à l'adresse fixe **`0x00000800`** et chargée avec `lgdt`
- Un outil de **stack dump** qui affiche la pile kernel façon `hexdump`

**Bonus**
- Un **shell** minimal in-kernel avec quelques commandes de debug

Tout KFS-1 fonctionne encore : boot GRUB, sortie VGA, scroll, curseur matériel,
couleurs, `printk`, clavier PS/2, et 3 TTY (**F1 / F2 / F3**).

---

## Le shell

Chaque TTY a son propre prompt (`kfs>`) et sa propre ligne de saisie.

| Commande | Effet                                                  |
|----------|--------------------------------------------------------|
| `stack`  | Dump de la pile kernel autour d'`esp`, façon hexdump   |
| `gdt`    | Affiche le GDTR (base/limite) et les registres de segment |
| `regs`   | Dump des registres généraux                            |
| `clear`  | Efface l'écran courant                                 |
| `reboot` | Redémarre la machine (reset CPU via le 8042)           |
| `halt`   | Arrête le CPU (`cli` + `hlt`)                           |

---

## Prérequis

Le kernel doit être compilé avec un **cross-compilateur i686-elf** (il ne peut pas
être lié à la libc de l'hôte). Il faut aussi les outils GRUB et un émulateur.

```sh
sudo ./scripts/install-deps.sh   # paquets hôte (gcc, grub, xorriso, qemu, ...)
./scripts/build-i686-elf.sh      # construit i686-elf-gcc dans $HOME/opt/cross
```

`install-deps.sh` gère apt / dnf / pacman. `build-i686-elf.sh` se lance en
utilisateur normal (installe dans un préfixe utilisateur).

---

## Compiler & lancer

```sh
make        # construit kfs.iso (par défaut)
make run    # démarre l'ISO dans QEMU
```

| Cible    | Description                                     |
|----------|-------------------------------------------------|
| `make` / `make all` | Construit l'ISO bootable (`kfs.iso`)     |
| `make kernel`       | Construit seulement le binaire (`kfs.bin`) |
| `make iso`          | Construit l'ISO bootable GRUB            |
| `make run`          | Démarre l'ISO dans `qemu-system-i386`    |
| `make clean`        | Supprime les objets et le staging ISO    |
| `make fclean`       | `clean` + supprime `kfs.bin` / `kfs.iso` |
| `make re`           | Recompilation complète                   |

---

## Organisation du projet

```
.
├── Makefile / mk/          # racine minimaliste + fragments config/rules/toolchain
├── include/                # headers publics (un par module)
│   └── gdt.h  debug.h  shell.h  vga.h  keyboard.h  ...
├── src/
│   ├── boot/               # boot.s (Multiboot + _start) + linker.ld
│   ├── kernel/             # kernel_main
│   ├── gdt/                # gdt.c (construit à 0x800) + gdt_flush.s (lgdt + reload)
│   ├── debug/              # stack_dump, gdt_debug, regs
│   ├── shell/              # shell.c, dispatch.c, cmd/ (reboot, halt)
│   ├── lib/                # mini-libc freestanding (str/ put/ fmt/)
│   └── drivers/            # vga/ (+ cursor/ keyboard/)
├── grub/grub.cfg
└── scripts/
```

Chaque dossier `src/**` porte un `module.mk` qui liste ses sources explicitement
(pas de wildcard) ; une ligne `include` dans le `Makefile` racine le branche.

---

## Comment ça démarre

1. **GRUB** trouve le **header Multiboot** dans `boot.s` et charge le kernel à 2 Mio.
2. **`_start`** met en place une pile et appelle `kernel_main`.
3. **`kernel_main`** lance **`gdt_init()` en premier** (construit la GDT à `0x800`,
   la charge avec `lgdt`, recharge les registres de segment via un far jump), puis
   initialise le terminal et démarre le shell.

---

## La GDT, en bref

La GDT est une table que le CPU lit pour traduire un **sélecteur de segment**
(dans `CS`, `DS`, `SS`, …) en la **base, la limite et les droits** d'une zone
mémoire (kernel/user, code/data). On utilise un **flat model** : chaque segment a
une base 0 et une limite de 4 Gio, donc la segmentation est transparente — la GDT
sert surtout à définir la frontière de privilège ring 0 / ring 3. La vraie
séparation mémoire par région viendra avec le paging (KFS-3).

### Vérifier la GDT

Le contrôle le plus fiable, c'est le monitor de QEMU. Dans la fenêtre QEMU, appuie
sur **Ctrl+Alt+2** (ou lance avec `-monitor stdio`) et tape :

```
info registers
```

Tu dois voir la GDT chargée à notre adresse :

```
CS =0008 00000000 ffffffff 00cf9a00 DPL=0 CS32 [-R-]
DS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
GDT=     00000800 00000037
```

- `GDT= 00000800 00000037` → base `0x800`, limite 55 (7 entrées)
- `CS=0008`, `DS=0010` → registres de segment rechargés sur nos descripteurs
- `CPL=0` → on tourne en ring 0

La commande `gdt` du kernel affiche les mêmes infos depuis l'intérieur.

---

## Notes

- **Architecture :** i386 (x86, 32 bits) — obligatoire pour la série.
- **Freestanding :** compilé avec `-ffreestanding -nostdlib -nodefaultlibs` ; pas
  de libc hôte, le kernel fournit donc ses propres `strlen`, `printk`, etc.
- **L'image doit rester sous 10 Mo** (exigence du sujet).
