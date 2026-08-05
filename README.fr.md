# KFS-1 — Kernel From Scratch

*Lire en [anglais](README.md).*

Un **kernel 32 bits (i386)** minimal qui démarre via **GRUB**, écrit de zéro
sans aucune bibliothèque de l'hôte. C'est le premier projet de la série 42
*Kernel From Scratch* : boot, affichage écran et une petite bibliothèque
freestanding.

---

## Ce que ça fait

Démarre via GRUB (Multiboot), met en place une pile, saute dans `kernel_main`
et écrit dans le buffer texte VGA.

**Mandatory**
- Bootable via GRUB (header Multiboot)
- Point d'entrée ASM + script de link maison
- Bibliothèque freestanding minimale (`strlen`, `strcmp`, `printk`, …)
- Écrit des caractères à l'écran — affiche `42`

**Bonus**
- Scroll et curseur matériel
- Couleurs
- `printk` (`%d %s %x %c %p`)
- Saisie clavier PS/2
- 3 TTY virtuels, bascule avec **F1 / F2 / F3**

---

## Prérequis

Le kernel doit être compilé avec un **cross-compilateur i686-elf** (il ne peut
pas être lié à la libc de l'hôte). Il faut aussi les outils GRUB et un émulateur.

Deux scripts s'occupent de l'installation :

```sh
sudo ./scripts/install-deps.sh   # paquets hôte (gcc, grub, xorriso, qemu, ...)
./scripts/build-i686-elf.sh      # construit i686-elf-gcc dans $HOME/opt/cross
```

`install-deps.sh` gère apt / dnf / pacman. `build-i686-elf.sh` se lance en
utilisateur normal (il installe dans un préfixe utilisateur).

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

Dans QEMU : tape au clavier pour écrire, appuie sur **F1/F2/F3** pour changer
de TTY.

---

## Organisation du projet

```
.
├── Makefile              # racine minimaliste : câble tout + cibles haut niveau
├── mk/                   # fragments de Makefile
│   ├── config.mk         #   toolchain, chemins, flags compil/link
│   ├── rules.mk          #   règles build/link/iso + auto-dépendances headers
│   └── toolchain.mk      #   vérif du cross-compilateur
├── include/              # headers publics (un par module)
│   ├── types.h  io.h  str.h  put.h  fmt.h
│   ├── vga.h  cursor.h  keyboard.h
│   └── kernel.h          #   header parapluie + gardes compilateur
├── src/
│   ├── boot/             # boot.s (Multiboot + _start) + linker.ld
│   ├── kernel/           # kernel_main
│   ├── lib/              # mini-libc freestanding, une fonction par fichier
│   │   ├── str/          #   strlen, strcmp
│   │   ├── put/          #   putchar, putstr, putnbr, puthex, putptr
│   │   └── fmt/          #   printk
│   └── drivers/
│       ├── vga/          #   terminal texte + TTY (+ vga_internal.h privé)
│       ├── cursor/       #   curseur matériel VGA
│       └── keyboard/     #   clavier PS/2 (polling)
├── grub/grub.cfg         # entrée de menu GRUB
└── scripts/              # setup dépendances + cross-compilateur
```

Chaque dossier `src/**` porte un `module.mk` qui liste ses sources
explicitement (pas de wildcard). Ajouter un module = créer son `module.mk` et
ajouter une ligne `include` dans le `Makefile` racine.

---

## Comment ça démarre

1. **GRUB** trouve le **header Multiboot** (magic `0x1BADB002`) que `boot.s`
   place tout au début du binaire, et charge le kernel à **2 Mio**
   (`linker.ld`).
2. **`_start`** (dans `boot.s`) met en place une pile de 16 Kio, puis
   `call kernel_main`.
3. **`kernel_main`** initialise le terminal et écrit dans le buffer texte VGA à
   `0xB8000`, puis boucle en interrogeant le clavier.

Le script de link (`src/boot/linker.ld`) définit le point d'entrée et place
`.multiboot` + `.text` en premier pour que GRUB reconnaisse l'image.

---

## Notes

- **Architecture :** i386 (x86, 32 bits) — obligatoire pour la série.
- **Freestanding :** compilé avec `-ffreestanding -nostdlib -nodefaultlibs` ;
  pas de libc hôte, le kernel fournit donc ses propres `strlen`, `printk`, etc.
- **L'image doit rester sous 10 Mo** (exigence du sujet) — l'ISO fait ~4 Mo.
