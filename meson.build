project(
    'spacecorridor',
    'c',
    version: '0.1',
    default_options: ['warning_level=3'],
)

sdl2 = dependency('sdl2', required: true)
sdl2_image = dependency('sdl2_image', required: true)
sdl2_ttf = dependency('sdl2_ttf', required: true)
sdl2_mixer = dependency('SDL2_mixer', required: true)

executable(
    'spacecorridor',
    [
        'src/sdl2-light.c',
        'src/main.c',
        'src/game.c',
        'src/graphics.c',
        'src/level.c',
        'src/sdl2-ttf-light.c',
    ],
    dependencies: [sdl2, sdl2_image, sdl2_ttf, sdl2_mixer],
    install: true,
)

doxygen = find_program('doxygen', required: false)

if doxygen.found()
    doxyfile = configure_file(
        input: 'Doxyfile.in',
        output: 'Doxyfile',
        configuration: {
            'SOURCE_DIR': meson.current_source_dir(),
            'BUILD_DIR': meson.current_build_dir(),
        },
    )

    custom_target(
        'doxygen',
        input: doxyfile,
        output: 'docs',
        command: [doxygen, '@INPUT@'],
        build_by_default: false,
        install: false,
        console: true,
    )
else
    message('Doxygen not found, skipping `doxygen` target.')
endif
