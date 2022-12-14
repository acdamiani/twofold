project(Twofold)

set(TARGET_SRC ${CMAKE_SOURCE_DIR}/src/twofold.cpp
               ${CMAKE_SOURCE_DIR}/src/audio.cpp
               ${CMAKE_SOURCE_DIR}/src/transform.cpp
               ${CMAKE_SOURCE_DIR}/src/window.cpp
)

set(TARGET_H   ${CMAKE_SOURCE_DIR}/include/audio.h
               ${CMAKE_SOURCE_DIR}/include/transform.h
               ${CMAKE_SOURCE_DIR}/include/window.h
)

find_package(
    FFTW REQUIRED
    COMPONENTS FLOAT_LIB DOUBLE_LIB LONGDOUBLE_LIB
)

find_package(PulseAudio REQUIRED)

add_executable(twofold ${TARGET_SRC} ${TARGET_H})

target_include_directories(twofold PUBLIC ${CMAKE_SOURCE_DIR}/include)

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

target_link_libraries(
  twofold
  gnuplot_iostream
)

add_custom_command(
    TARGET twofold
    POST_BUILD
    COMMAND twofold
)
