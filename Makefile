# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = "/mnt/c/Users/mylaptop/Google Drive/3DProjects"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/mylaptop/Google Drive/3DProjects"

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start "/mnt/c/Users/mylaptop/Google Drive/3DProjects/CMakeFiles" "/mnt/c/Users/mylaptop/Google Drive/3DProjects/CMakeFiles/progress.marks"
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start "/mnt/c/Users/mylaptop/Google Drive/3DProjects/CMakeFiles" 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named 3dConverter

# Build rule for target.
3dConverter: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 3dConverter
.PHONY : 3dConverter

# fast build rule for target.
3dConverter/fast:
	$(MAKE) -f CMakeFiles/3dConverter.dir/build.make CMakeFiles/3dConverter.dir/build
.PHONY : 3dConverter/fast

3dConverter.o: 3dConverter.cpp.o

.PHONY : 3dConverter.o

# target to build an object file
3dConverter.cpp.o:
	$(MAKE) -f CMakeFiles/3dConverter.dir/build.make CMakeFiles/3dConverter.dir/3dConverter.cpp.o
.PHONY : 3dConverter.cpp.o

3dConverter.i: 3dConverter.cpp.i

.PHONY : 3dConverter.i

# target to preprocess a source file
3dConverter.cpp.i:
	$(MAKE) -f CMakeFiles/3dConverter.dir/build.make CMakeFiles/3dConverter.dir/3dConverter.cpp.i
.PHONY : 3dConverter.cpp.i

3dConverter.s: 3dConverter.cpp.s

.PHONY : 3dConverter.s

# target to generate assembly for a file
3dConverter.cpp.s:
	$(MAKE) -f CMakeFiles/3dConverter.dir/build.make CMakeFiles/3dConverter.dir/3dConverter.cpp.s
.PHONY : 3dConverter.cpp.s

Convert2AssImp.o: Convert2AssImp.cpp.o

.PHONY : Convert2AssImp.o

# target to build an object file
Convert2AssImp.cpp.o:
	$(MAKE) -f CMakeFiles/3dConverter.dir/build.make CMakeFiles/3dConverter.dir/Convert2AssImp.cpp.o
.PHONY : Convert2AssImp.cpp.o

Convert2AssImp.i: Convert2AssImp.cpp.i

.PHONY : Convert2AssImp.i

# target to preprocess a source file
Convert2AssImp.cpp.i:
	$(MAKE) -f CMakeFiles/3dConverter.dir/build.make CMakeFiles/3dConverter.dir/Convert2AssImp.cpp.i
.PHONY : Convert2AssImp.cpp.i

Convert2AssImp.s: Convert2AssImp.cpp.s

.PHONY : Convert2AssImp.s

# target to generate assembly for a file
Convert2AssImp.cpp.s:
	$(MAKE) -f CMakeFiles/3dConverter.dir/build.make CMakeFiles/3dConverter.dir/Convert2AssImp.cpp.s
.PHONY : Convert2AssImp.cpp.s

IfcAssImpImporter.o: IfcAssImpImporter.cpp.o

.PHONY : IfcAssImpImporter.o

# target to build an object file
IfcAssImpImporter.cpp.o:
	$(MAKE) -f CMakeFiles/3dConverter.dir/build.make CMakeFiles/3dConverter.dir/IfcAssImpImporter.cpp.o
.PHONY : IfcAssImpImporter.cpp.o

IfcAssImpImporter.i: IfcAssImpImporter.cpp.i

.PHONY : IfcAssImpImporter.i

# target to preprocess a source file
IfcAssImpImporter.cpp.i:
	$(MAKE) -f CMakeFiles/3dConverter.dir/build.make CMakeFiles/3dConverter.dir/IfcAssImpImporter.cpp.i
.PHONY : IfcAssImpImporter.cpp.i

IfcAssImpImporter.s: IfcAssImpImporter.cpp.s

.PHONY : IfcAssImpImporter.s

# target to generate assembly for a file
IfcAssImpImporter.cpp.s:
	$(MAKE) -f CMakeFiles/3dConverter.dir/build.make CMakeFiles/3dConverter.dir/IfcAssImpImporter.cpp.s
.PHONY : IfcAssImpImporter.cpp.s

assimpConverter.o: assimpConverter.cpp.o

.PHONY : assimpConverter.o

# target to build an object file
assimpConverter.cpp.o:
	$(MAKE) -f CMakeFiles/3dConverter.dir/build.make CMakeFiles/3dConverter.dir/assimpConverter.cpp.o
.PHONY : assimpConverter.cpp.o

assimpConverter.i: assimpConverter.cpp.i

.PHONY : assimpConverter.i

# target to preprocess a source file
assimpConverter.cpp.i:
	$(MAKE) -f CMakeFiles/3dConverter.dir/build.make CMakeFiles/3dConverter.dir/assimpConverter.cpp.i
.PHONY : assimpConverter.cpp.i

assimpConverter.s: assimpConverter.cpp.s

.PHONY : assimpConverter.s

# target to generate assembly for a file
assimpConverter.cpp.s:
	$(MAKE) -f CMakeFiles/3dConverter.dir/build.make CMakeFiles/3dConverter.dir/assimpConverter.cpp.s
.PHONY : assimpConverter.cpp.s

myIoStream.o: myIoStream.cpp.o

.PHONY : myIoStream.o

# target to build an object file
myIoStream.cpp.o:
	$(MAKE) -f CMakeFiles/3dConverter.dir/build.make CMakeFiles/3dConverter.dir/myIoStream.cpp.o
.PHONY : myIoStream.cpp.o

myIoStream.i: myIoStream.cpp.i

.PHONY : myIoStream.i

# target to preprocess a source file
myIoStream.cpp.i:
	$(MAKE) -f CMakeFiles/3dConverter.dir/build.make CMakeFiles/3dConverter.dir/myIoStream.cpp.i
.PHONY : myIoStream.cpp.i

myIoStream.s: myIoStream.cpp.s

.PHONY : myIoStream.s

# target to generate assembly for a file
myIoStream.cpp.s:
	$(MAKE) -f CMakeFiles/3dConverter.dir/build.make CMakeFiles/3dConverter.dir/myIoStream.cpp.s
.PHONY : myIoStream.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... 3dConverter"
	@echo "... 3dConverter.o"
	@echo "... 3dConverter.i"
	@echo "... 3dConverter.s"
	@echo "... Convert2AssImp.o"
	@echo "... Convert2AssImp.i"
	@echo "... Convert2AssImp.s"
	@echo "... IfcAssImpImporter.o"
	@echo "... IfcAssImpImporter.i"
	@echo "... IfcAssImpImporter.s"
	@echo "... assimpConverter.o"
	@echo "... assimpConverter.i"
	@echo "... assimpConverter.s"
	@echo "... myIoStream.o"
	@echo "... myIoStream.i"
	@echo "... myIoStream.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

