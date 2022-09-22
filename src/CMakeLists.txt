project(Twofold)

set(TARGET_SRC ${CMAKE_SOURCE_DIR}/src/twofold.cpp
               ${CMAKE_SOURCE_DIR}/src/audio.cpp
)

set(TARGET_H   ${CMAKE_SOURCE_DIR}/include/audio.h
)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(twofold ${TARGET_SRC} ${TARGET_H})

add_custom_command(
    TARGET twofold
    POST_BUILD
    COMMAND twofold
)
