project(Twofold)

target_include_directories(twofold PUBLIC ${CMAKE_SOURCE_DIR}/third_party/gnuplot-iostream/)

add_subdirectory(gnuplot-iostream)
