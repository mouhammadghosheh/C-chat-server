# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/mouha/CLionProjects/chat

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/mouha/CLionProjects/chat/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/chat.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/chat.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/chat.dir/flags.make

CMakeFiles/chat.dir/chatServer.c.o: CMakeFiles/chat.dir/flags.make
CMakeFiles/chat.dir/chatServer.c.o: ../chatServer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/mouha/CLionProjects/chat/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/chat.dir/chatServer.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/chat.dir/chatServer.c.o   -c /mnt/c/Users/mouha/CLionProjects/chat/chatServer.c

CMakeFiles/chat.dir/chatServer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/chat.dir/chatServer.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/mouha/CLionProjects/chat/chatServer.c > CMakeFiles/chat.dir/chatServer.c.i

CMakeFiles/chat.dir/chatServer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/chat.dir/chatServer.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/mouha/CLionProjects/chat/chatServer.c -o CMakeFiles/chat.dir/chatServer.c.s

# Object files for target chat
chat_OBJECTS = \
"CMakeFiles/chat.dir/chatServer.c.o"

# External object files for target chat
chat_EXTERNAL_OBJECTS =

chat: CMakeFiles/chat.dir/chatServer.c.o
chat: CMakeFiles/chat.dir/build.make
chat: CMakeFiles/chat.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/mouha/CLionProjects/chat/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable chat"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chat.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/chat.dir/build: chat

.PHONY : CMakeFiles/chat.dir/build

CMakeFiles/chat.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/chat.dir/cmake_clean.cmake
.PHONY : CMakeFiles/chat.dir/clean

CMakeFiles/chat.dir/depend:
	cd /mnt/c/Users/mouha/CLionProjects/chat/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/mouha/CLionProjects/chat /mnt/c/Users/mouha/CLionProjects/chat /mnt/c/Users/mouha/CLionProjects/chat/cmake-build-debug /mnt/c/Users/mouha/CLionProjects/chat/cmake-build-debug /mnt/c/Users/mouha/CLionProjects/chat/cmake-build-debug/CMakeFiles/chat.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/chat.dir/depend
