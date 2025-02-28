project('arcade', 'c', version: '0.1', default_options: ['warning_level=3'])

sdl2 = dependency('sdl2', required: true)

executable(
    'arcade',
    ['src/sdl2-light.c', 'src/main.c'],
    dependencies: [sdl2],
    install: true,
)
