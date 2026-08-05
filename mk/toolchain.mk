# ============================================================================
#  mk/toolchain.mk - verify the i686-elf cross-compiler is available.
# ============================================================================

.PHONY: check-toolchain
check-toolchain:
	@command -v $(CC) >/dev/null 2>&1 || { \
		echo "error: $(CC) not found. Set up the toolchain first:"; \
		echo "  sudo ./scripts/install-deps.sh   # host build tools"; \
		echo "  ./scripts/build-i686-elf.sh      # the cross-compiler"; \
		echo "or add its bin/ to PATH (PREFIX=$(PREFIX))."; \
		exit 1; \
	}
