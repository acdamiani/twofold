project(Twofold)

set(TARGET_SRC ${CMAKE_SOURCE_DIR}/src/twofold.cpp
               ${CMAKE_SOURCE_DIR}/src/audio.cpp
)

set(TARGET_H   ${CMAKE_SOURCE_DIR}/include/audio.h
)

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(
    FFTW REQUIRED
    COMPONENTS FLOAT_LIB DOUBLE_LIB
)

find_package(PulseAudio REQUIRED)

add_executable(twofold ${TARGET_SRC} ${TARGET_H})

message(INFO ${PULSEAUDIO_MAINLOOP_LIBRARY})

target_link_libraries(
    twofold
    FFTW::Float
    FFTW::Double
    FFTW::LongDouble
)

target_link_libraries(
  twofold
  ${PULSEAUDIO_LIBRARY}
  pulse-simple
)

add_custom_command(
    TARGET twofold
    POST_BUILD
    COMMAND twofold
)
