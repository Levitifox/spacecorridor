build: builddir
	meson compile -C builddir

builddir:
	meson setup builddir

run: build
	./builddir/arcade

clean:
	rm -rf builddir

.PHONY: build run clean
