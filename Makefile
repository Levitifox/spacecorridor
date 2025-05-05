build: builddir
	meson compile -C builddir

builddir:
	meson setup builddir

run: build
	./builddir/spacecorridor

fmt:
	meson format -ir
	find src -iname '*.h' -o -iname '*.c' | xargs clang-format -i

clean:
	rm -rf builddir

.PHONY: build run clean
