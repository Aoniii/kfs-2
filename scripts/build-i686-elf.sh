#!/usr/bin/env bash
# Build an i686-elf GCC cross-compiler (OSDev Bare Bones style).
# Installs to $HOME/opt/cross by default.
#
# Usage:
#   ./scripts/build-i686-elf.sh
#   PREFIX=/opt/cross ./scripts/build-i686-elf.sh
#
# After install, add to ~/.zshrc:
#   export PATH="$HOME/opt/cross/bin:$PATH"

set -euo pipefail

PREFIX="${PREFIX:-$HOME/opt/cross}"
TARGET="${TARGET:-i686-elf}"
SRC_DIR="${SRC_DIR:-$PWD/i686-elf}"
JOBS="${JOBS:-$(nproc)}"

# Versions known to work together (OSDev Successful Builds).
BINUTILS_VERSION="${BINUTILS_VERSION:-2.43.1}"
GCC_VERSION="${GCC_VERSION:-14.2.0}"

BINUTILS_TAR="binutils-${BINUTILS_VERSION}.tar.xz"
GCC_TAR="gcc-${GCC_VERSION}.tar.xz"
BINUTILS_URL="https://ftp.gnu.org/gnu/binutils/${BINUTILS_TAR}"
GCC_URL="https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/${GCC_TAR}"

export PATH="${PREFIX}/bin:${PATH}"
export PREFIX TARGET

log() { printf '\n==> %s\n' "$*"; }
die() { printf 'error: %s\n' "$*" >&2; exit 1; }

need_cmd() {
	command -v "$1" >/dev/null 2>&1 || die "missing command: $1"
}

check_host_deps() {
	log "Checking host tools"
	local cmd
	# makeinfo/texinfo is optional: docs are stubbed if missing.
	# gmp/mpfr/mpc come from gcc's download_prerequisites (no -dev packages needed).
	for cmd in make gcc g++ bison flex curl tar xz; do
		need_cmd "$cmd"
	done

	if ! command -v makeinfo >/dev/null 2>&1; then
		log "makeinfo not found — disabling documentation build"
		export MAKEINFO=true
	fi
}

download() {
	local url="$1"
	local out="$2"
	if [[ -f "$out" ]]; then
		log "Already downloaded: $out"
		return
	fi
	log "Downloading $url"
	curl -L --fail --progress-bar -o "$out" "$url"
}

extract() {
	local tarfile="$1"
	local destdir="$2"
	if [[ -d "$destdir" ]]; then
		log "Already extracted: $destdir"
		return
	fi
	log "Extracting $tarfile"
	tar -xf "$tarfile" -C "$(dirname "$destdir")"
}

build_binutils() {
	local src="$SRC_DIR/binutils-${BINUTILS_VERSION}"
	local build="$SRC_DIR/build-binutils"

	if [[ -x "${PREFIX}/bin/${TARGET}-ld" ]]; then
		log "Binutils already installed (${TARGET}-ld found), skipping"
		return
	fi

	rm -rf "$build"
	mkdir -p "$build"
	cd "$build"

	log "Configuring binutils ${BINUTILS_VERSION}"
	"$src/configure" \
		--target="$TARGET" \
		--prefix="$PREFIX" \
		--with-sysroot \
		--disable-nls \
		--disable-werror \
		MAKEINFO="${MAKEINFO:-makeinfo}"

	log "Building binutils (-j${JOBS})"
	make -j"${JOBS}" MAKEINFO="${MAKEINFO:-makeinfo}"
	make install MAKEINFO="${MAKEINFO:-makeinfo}"
}

build_gcc() {
	local src="$SRC_DIR/gcc-${GCC_VERSION}"
	local build="$SRC_DIR/build-gcc"

	if [[ -x "${PREFIX}/bin/${TARGET}-gcc" ]]; then
		log "GCC already installed (${TARGET}-gcc found), skipping"
		return
	fi

	# Download GCC prerequisites (gmp/mpfr/mpc) into the source tree.
	if [[ ! -d "$src/gmp" && ! -d "$src/mpfr" ]]; then
		log "Fetching GCC prerequisites (gmp/mpfr/mpc)"
		cd "$src"
		./contrib/download_prerequisites
	fi

	rm -rf "$build"
	mkdir -p "$build"
	cd "$build"

	log "Configuring gcc ${GCC_VERSION}"
	"$src/configure" \
		--target="$TARGET" \
		--prefix="$PREFIX" \
		--disable-nls \
		--enable-languages=c,c++ \
		--without-headers \
		--disable-hosted-libstdcxx \
		--disable-shared \
		--disable-threads \
		--disable-tls \
		--disable-libssp \
		--disable-libgomp \
		--disable-libquadmath \
		--disable-libatomic \
		MAKEINFO="${MAKEINFO:-makeinfo}"

	log "Building gcc (-j${JOBS}) — this takes a while"
	make -j"${JOBS}" MAKEINFO="${MAKEINFO:-makeinfo}" all-gcc
	make -j"${JOBS}" MAKEINFO="${MAKEINFO:-makeinfo}" all-target-libgcc
	make install-gcc MAKEINFO="${MAKEINFO:-makeinfo}"
	make install-target-libgcc MAKEINFO="${MAKEINFO:-makeinfo}"
}

verify() {
	log "Verifying toolchain"
	need_cmd "${TARGET}-gcc"
	need_cmd "${TARGET}-ld"
	need_cmd "${TARGET}-as"

	"${TARGET}-gcc" --version
	"${TARGET}-ld" --version | head -n1

	# Sanity: produce a freestanding object, not a Linux binary.
	local tmp
	tmp="$(mktemp -d)"
	cat >"$tmp/test.c" <<'EOF'
void _start(void) {}
EOF
	"${TARGET}-gcc" -ffreestanding -c "$tmp/test.c" -o "$tmp/test.o"
	file "$tmp/test.o"
	rm -rf "$tmp"

	log "OK — toolchain ready in ${PREFIX}"
}

main() {
	log "i686-elf cross-compiler build"
	echo "PREFIX=${PREFIX}"
	echo "TARGET=${TARGET}"
	echo "BINUTILS=${BINUTILS_VERSION}"
	echo "GCC=${GCC_VERSION}"
	echo "SRC_DIR=${SRC_DIR}"
	echo "JOBS=${JOBS}"

	check_host_deps
	mkdir -p "$PREFIX" "$SRC_DIR"
	cd "$SRC_DIR"

	download "$BINUTILS_URL" "$SRC_DIR/$BINUTILS_TAR"
	download "$GCC_URL" "$SRC_DIR/$GCC_TAR"

	extract "$SRC_DIR/$BINUTILS_TAR" "$SRC_DIR/binutils-${BINUTILS_VERSION}"
	extract "$SRC_DIR/$GCC_TAR" "$SRC_DIR/gcc-${GCC_VERSION}"

	build_binutils
	build_gcc
	verify

	cat <<EOF

Add this to your ~/.zshrc:

  export PATH="${PREFIX}/bin:\$PATH"

Then reload:

  source ~/.zshrc

Test:

  ${TARGET}-gcc --version
EOF
}

main "$@"
