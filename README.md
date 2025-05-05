Spacecorridor
===

## Installation of dependencies

On Windows, install MSYS2 and then run in the MSYS2 MSYS shell:
```powershell
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-clang mingw-w64-x86_64-meson mingw-w64-x86_64-gdb mingw-w64-x86_64-python-pygments
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_gfx mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-SDL2_net mingw-w64-x86_64-SDL2_pango mingw-w64-x86_64-SDL2_sound mingw-w64-x86_64-SDL2_ttf
```

On Ubuntu:
```sh
apt install build-essential clang git pkg-config meson
apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-net-dev libsdl2-mixer-dev libsdl2-gfx-dev
```

## Building

Use either Linux (or similar) or MSYS2 WINGW64 environment; run:
```sh
meson setup builddir
meson compile
```

Or just:
```sh
make
```

## Running

Run the `./builddir/spacecorridor` executable, or use:
```sh
make run
```
