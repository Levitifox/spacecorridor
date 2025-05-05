build: builddir
	meson compile -C builddir

builddir:
	meson setup builddir

run: build
	./builddir/spacecorridor

fmt:
	meson format -ir
	find src -iname '*.h' -o -iname '*.c' | xargs clang-format -i

dist: build
	rm -rf dist
	mkdir -p dist
	cp -r resources dist
	cp builddir/spacecorridor.exe dist/
	ldd dist/spacecorridor.exe | grep "mingw64/bin/" | sed -E 's/^[^>]*=> ([^ ]+) .*/\1/' | xargs -I{} cp {} dist/

doxygen: builddir
	meson compile -C builddir doxygen

clean:
	rm -rf builddir dist

.PHONY: build run fmt dist doxygen clean
