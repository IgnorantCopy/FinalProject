cmake_minimum_required(VERSION 3.26)
project(FinalProject C)

set(CMAKE_C_STANDARD 11)

include_directories("../../Import/mingw(CLion+VSC)/SDL2-2.26.0-allinone/x86_64-w64-mingw32/include")
link_directories("../../Import/mingw(CLion+VSC)/SDL2-2.26.0-allinone/x86_64-w64-mingw32/bin")

add_subdirectory(MyParkour)

target_link_libraries(SDLTest PRIVATE
        mingw32
        SDL2main
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf
)

target_link_libraries(Parkour PRIVATE
        mingw32
        SDL2main
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf
)

