# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_COMMAND = /home/dai/commom/cmake-3.23.0-rc1-linux-x86_64/bin/cmake

# The command to remove a file.
RM = /home/dai/commom/cmake-3.23.0-rc1-linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/media/dai/Seagate Basic1/学习总文件夹/网课/线程和线程池/code"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/media/dai/Seagate Basic1/学习总文件夹/网课/线程和线程池/code/build"

# Include any dependencies generated for this target.
include CMakeFiles/04.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/04.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/04.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/04.dir/flags.make

CMakeFiles/04.dir/cpp/04.cpp.o: CMakeFiles/04.dir/flags.make
CMakeFiles/04.dir/cpp/04.cpp.o: ../cpp/04.cpp
CMakeFiles/04.dir/cpp/04.cpp.o: CMakeFiles/04.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/media/dai/Seagate Basic1/学习总文件夹/网课/线程和线程池/code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/04.dir/cpp/04.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/04.dir/cpp/04.cpp.o -MF CMakeFiles/04.dir/cpp/04.cpp.o.d -o CMakeFiles/04.dir/cpp/04.cpp.o -c "/media/dai/Seagate Basic1/学习总文件夹/网课/线程和线程池/code/cpp/04.cpp"

CMakeFiles/04.dir/cpp/04.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/04.dir/cpp/04.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/dai/Seagate Basic1/学习总文件夹/网课/线程和线程池/code/cpp/04.cpp" > CMakeFiles/04.dir/cpp/04.cpp.i

CMakeFiles/04.dir/cpp/04.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/04.dir/cpp/04.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/dai/Seagate Basic1/学习总文件夹/网课/线程和线程池/code/cpp/04.cpp" -o CMakeFiles/04.dir/cpp/04.cpp.s

# Object files for target 04
04_OBJECTS = \
"CMakeFiles/04.dir/cpp/04.cpp.o"

# External object files for target 04
04_EXTERNAL_OBJECTS =

bin/04: CMakeFiles/04.dir/cpp/04.cpp.o
bin/04: CMakeFiles/04.dir/build.make
bin/04: CMakeFiles/04.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/media/dai/Seagate Basic1/学习总文件夹/网课/线程和线程池/code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/04"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/04.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/04.dir/build: bin/04
.PHONY : CMakeFiles/04.dir/build

CMakeFiles/04.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/04.dir/cmake_clean.cmake
.PHONY : CMakeFiles/04.dir/clean

CMakeFiles/04.dir/depend:
	cd "/media/dai/Seagate Basic1/学习总文件夹/网课/线程和线程池/code/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/media/dai/Seagate Basic1/学习总文件夹/网课/线程和线程池/code" "/media/dai/Seagate Basic1/学习总文件夹/网课/线程和线程池/code" "/media/dai/Seagate Basic1/学习总文件夹/网课/线程和线程池/code/build" "/media/dai/Seagate Basic1/学习总文件夹/网课/线程和线程池/code/build" "/media/dai/Seagate Basic1/学习总文件夹/网课/线程和线程池/code/build/CMakeFiles/04.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/04.dir/depend

