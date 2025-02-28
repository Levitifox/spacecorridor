arcade
===

## Installation of dependencies

For Windows, install MSYS2 and then run:

```powershell
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-meson mingw-w64-x86_64-gdb mingw-w64-x86_64-python-pygments
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_gfx mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-SDL2_net mingw-w64-x86_64-SDL2_pango mingw-w64-x86_64-SDL2_sound mingw-w64-x86_64-SDL2_ttf
```

## Building

```
meson setup builddir
meson compile
```

Or just:
```
make
```

