add_executable(cJSON cJSON.c)
add_executable(Parkour
        main.c
        menu.c
        utils/audio.c
        utils/input.c
        utils/display.c
        game/game.c
        ../include/game/game.h
)