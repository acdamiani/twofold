# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/acdamiani/Documents/twofold

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/acdamiani/Documents/twofold/build

# Include any dependencies generated for this target.
include src/CMakeFiles/twofold.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/twofold.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/twofold.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/twofold.dir/flags.make

src/CMakeFiles/twofold.dir/twofold.cpp.o: src/CMakeFiles/twofold.dir/flags.make
src/CMakeFiles/twofold.dir/twofold.cpp.o: /home/acdamiani/Documents/twofold/src/twofold.cpp
src/CMakeFiles/twofold.dir/twofold.cpp.o: src/CMakeFiles/twofold.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/acdamiani/Documents/twofold/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/twofold.dir/twofold.cpp.o"
	cd /home/acdamiani/Documents/twofold/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/twofold.dir/twofold.cpp.o -MF CMakeFiles/twofold.dir/twofold.cpp.o.d -o CMakeFiles/twofold.dir/twofold.cpp.o -c /home/acdamiani/Documents/twofold/src/twofold.cpp

src/CMakeFiles/twofold.dir/twofold.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/twofold.dir/twofold.cpp.i"
	cd /home/acdamiani/Documents/twofold/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/acdamiani/Documents/twofold/src/twofold.cpp > CMakeFiles/twofold.dir/twofold.cpp.i

src/CMakeFiles/twofold.dir/twofold.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/twofold.dir/twofold.cpp.s"
	cd /home/acdamiani/Documents/twofold/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/acdamiani/Documents/twofold/src/twofold.cpp -o CMakeFiles/twofold.dir/twofold.cpp.s

src/CMakeFiles/twofold.dir/audio.cpp.o: src/CMakeFiles/twofold.dir/flags.make
src/CMakeFiles/twofold.dir/audio.cpp.o: /home/acdamiani/Documents/twofold/src/audio.cpp
src/CMakeFiles/twofold.dir/audio.cpp.o: src/CMakeFiles/twofold.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/acdamiani/Documents/twofold/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/twofold.dir/audio.cpp.o"
	cd /home/acdamiani/Documents/twofold/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/twofold.dir/audio.cpp.o -MF CMakeFiles/twofold.dir/audio.cpp.o.d -o CMakeFiles/twofold.dir/audio.cpp.o -c /home/acdamiani/Documents/twofold/src/audio.cpp

src/CMakeFiles/twofold.dir/audio.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/twofold.dir/audio.cpp.i"
	cd /home/acdamiani/Documents/twofold/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/acdamiani/Documents/twofold/src/audio.cpp > CMakeFiles/twofold.dir/audio.cpp.i

src/CMakeFiles/twofold.dir/audio.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/twofold.dir/audio.cpp.s"
	cd /home/acdamiani/Documents/twofold/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/acdamiani/Documents/twofold/src/audio.cpp -o CMakeFiles/twofold.dir/audio.cpp.s

# Object files for target twofold
twofold_OBJECTS = \
"CMakeFiles/twofold.dir/twofold.cpp.o" \
"CMakeFiles/twofold.dir/audio.cpp.o"

# External object files for target twofold
twofold_EXTERNAL_OBJECTS =

src/twofold: src/CMakeFiles/twofold.dir/twofold.cpp.o
src/twofold: src/CMakeFiles/twofold.dir/audio.cpp.o
src/twofold: src/CMakeFiles/twofold.dir/build.make
src/twofold: /usr/lib/libpulse.so
src/twofold: /usr/lib/libfftw3f.so
src/twofold: /usr/lib/libfftw3.so
src/twofold: /usr/lib/libfftw3l.so
src/twofold: src/CMakeFiles/twofold.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/acdamiani/Documents/twofold/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable twofold"
	cd /home/acdamiani/Documents/twofold/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/twofold.dir/link.txt --verbose=$(VERBOSE)
	cd /home/acdamiani/Documents/twofold/build/src && ./twofold

# Rule to build all files generated by this target.
src/CMakeFiles/twofold.dir/build: src/twofold
.PHONY : src/CMakeFiles/twofold.dir/build

src/CMakeFiles/twofold.dir/clean:
	cd /home/acdamiani/Documents/twofold/build/src && $(CMAKE_COMMAND) -P CMakeFiles/twofold.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/twofold.dir/clean

src/CMakeFiles/twofold.dir/depend:
	cd /home/acdamiani/Documents/twofold/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/acdamiani/Documents/twofold /home/acdamiani/Documents/twofold/src /home/acdamiani/Documents/twofold/build /home/acdamiani/Documents/twofold/build/src /home/acdamiani/Documents/twofold/build/src/CMakeFiles/twofold.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/twofold.dir/depend
