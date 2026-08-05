#!/usr/bin/env bash
# Install every HOST dependency needed to build and run the KFS kernel:
#   1. tools to build the i686-elf cross-compiler (see build-i686-elf.sh)
#   2. tools to assemble the bootable ISO (grub-mkrescue + xorriso + mtools)
#   3. an emulator to run it (qemu-system-i386)
#
# This installs the tools to *build* the cross-compiler, NOT the
# cross-compiler itself. Run build-i686-elf.sh afterwards for that.
#
# Must be run as root:
#   sudo ./scripts/install-deps.sh
#
# Supported distros: Debian/Ubuntu (apt), Fedora/RHEL (dnf), Arch (pacman).
#
# Note: nasm is intentionally NOT installed — boot.s is assembled with
# i686-elf-gcc (GAS/intel syntax), not nasm.

set -euo pipefail

log() { printf '\n==> %s\n' "$*"; }
warn() { printf 'warning: %s\n' "$*" >&2; }
die() {
    printf 'error: %s\n' "$*" >&2
    exit 1
}

# --- Must be root ----------------------------------------------------------
if [[ "${EUID:-$(id -u)}" -ne 0 ]]; then
    die "this script must be run as root: sudo $0"
fi

# --- Detect the package manager -------------------------------------------
detect_pm() {
    if command -v apt-get >/dev/null 2>&1; then
        echo apt
    elif command -v dnf >/dev/null 2>&1; then
        echo dnf
    elif command -v pacman >/dev/null 2>&1; then
        echo pacman
    else
        die "unsupported distro: need apt-get, dnf or pacman"
    fi
}

PM="$(detect_pm)"
log "Detected package manager: $PM"

# --- Install ---------------------------------------------------------------
# Package groups, per manager:
#   [cross-compiler build] gcc/g++/make, bison, flex, gmp/mpfr/mpc(-dev), texinfo
#   [download + verify]    curl, xz, tar, file
#   [iso build]            grub (BIOS modules), xorriso, mtools
#   [emulator]             qemu-system-x86 (provides qemu-system-i386)
case "$PM" in
apt)
    PKGS=(
        build-essential bison flex
        libgmp-dev libmpfr-dev libmpc-dev texinfo
        curl xz-utils tar file
        grub-pc-bin grub-common xorriso mtools
        qemu-system-x86
    )
    log "Updating package index"
    apt-get update
    log "Installing dependencies"
    DEBIAN_FRONTEND=noninteractive \
        apt-get install -y --no-install-recommends "${PKGS[@]}"
    ;;
dnf)
    PKGS=(
        gcc gcc-c++ make bison flex
        gmp-devel mpfr-devel libmpc-devel texinfo
        curl xz tar file
        grub2-tools grub2-tools-extra xorriso mtools
        qemu-system-x86
    )
    log "Installing dependencies"
    dnf install -y "${PKGS[@]}"
    warn "On Fedora the tool is 'grub2-mkrescue', not 'grub-mkrescue'."
    warn "Symlink it if the Makefile expects the plain name:"
    warn "  ln -sf \"\$(command -v grub2-mkrescue)\" /usr/local/bin/grub-mkrescue"
    ;;
pacman)
    PKGS=(
        base-devel bison flex
        gmp mpfr libmpc texinfo
        curl xz tar file
        grub xorriso mtools
        qemu-system-x86
    )
    log "Installing dependencies"
    pacman -Sy --needed --noconfirm "${PKGS[@]}"
    ;;
esac

# --- Verify ----------------------------------------------------------------
log "Verifying installed tools"
missing=0
check() {
    if command -v "$1" >/dev/null 2>&1; then
        printf '  ok   %s\n' "$1"
    else
        printf '  MISS %s\n' "$1"
        missing=1
    fi
}

for c in gcc g++ make bison flex curl xz tar file xorriso; do
    check "$c"
done

# grub-mkrescue is named grub2-mkrescue on Fedora
if command -v grub-mkrescue >/dev/null 2>&1 || command -v grub2-mkrescue >/dev/null 2>&1; then
    printf '  ok   grub-mkrescue\n'
else
    printf '  MISS grub-mkrescue\n'
    missing=1
fi

# qemu-system-i386 is provided by the qemu-system-x86 package
check qemu-system-i386

if [[ "$missing" -ne 0 ]]; then
    warn "some tools are missing - check the package names for your distro"
fi

cat <<'EOF'

==> Host dependencies installed.

This installed the tools to *build* the cross-compiler, not the
cross-compiler itself. Next steps:

  ./scripts/build-i686-elf.sh   # build i686-elf-gcc (run as normal user)
  make                          # build kfs.iso
  make run                      # boot it in qemu

EOF
