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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.16.5/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.16.5/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/mu_build

# Include any dependencies generated for this target.
include CMakeFiles/sim_MuonicAtom.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sim_MuonicAtom.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sim_MuonicAtom.dir/flags.make

CMakeFiles/sim_MuonicAtom.dir/MuonicAtom.cc.o: CMakeFiles/sim_MuonicAtom.dir/flags.make
CMakeFiles/sim_MuonicAtom.dir/MuonicAtom.cc.o: /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/MuonicAtom.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/mu_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sim_MuonicAtom.dir/MuonicAtom.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sim_MuonicAtom.dir/MuonicAtom.cc.o -c /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/MuonicAtom.cc

CMakeFiles/sim_MuonicAtom.dir/MuonicAtom.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sim_MuonicAtom.dir/MuonicAtom.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/MuonicAtom.cc > CMakeFiles/sim_MuonicAtom.dir/MuonicAtom.cc.i

CMakeFiles/sim_MuonicAtom.dir/MuonicAtom.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sim_MuonicAtom.dir/MuonicAtom.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/MuonicAtom.cc -o CMakeFiles/sim_MuonicAtom.dir/MuonicAtom.cc.s

CMakeFiles/sim_MuonicAtom.dir/src/ActionInitialization.cc.o: CMakeFiles/sim_MuonicAtom.dir/flags.make
CMakeFiles/sim_MuonicAtom.dir/src/ActionInitialization.cc.o: /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/ActionInitialization.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/mu_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/sim_MuonicAtom.dir/src/ActionInitialization.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sim_MuonicAtom.dir/src/ActionInitialization.cc.o -c /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/ActionInitialization.cc

CMakeFiles/sim_MuonicAtom.dir/src/ActionInitialization.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sim_MuonicAtom.dir/src/ActionInitialization.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/ActionInitialization.cc > CMakeFiles/sim_MuonicAtom.dir/src/ActionInitialization.cc.i

CMakeFiles/sim_MuonicAtom.dir/src/ActionInitialization.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sim_MuonicAtom.dir/src/ActionInitialization.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/ActionInitialization.cc -o CMakeFiles/sim_MuonicAtom.dir/src/ActionInitialization.cc.s

CMakeFiles/sim_MuonicAtom.dir/src/DetectorConstruction.cc.o: CMakeFiles/sim_MuonicAtom.dir/flags.make
CMakeFiles/sim_MuonicAtom.dir/src/DetectorConstruction.cc.o: /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/DetectorConstruction.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/mu_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/sim_MuonicAtom.dir/src/DetectorConstruction.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sim_MuonicAtom.dir/src/DetectorConstruction.cc.o -c /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/DetectorConstruction.cc

CMakeFiles/sim_MuonicAtom.dir/src/DetectorConstruction.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sim_MuonicAtom.dir/src/DetectorConstruction.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/DetectorConstruction.cc > CMakeFiles/sim_MuonicAtom.dir/src/DetectorConstruction.cc.i

CMakeFiles/sim_MuonicAtom.dir/src/DetectorConstruction.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sim_MuonicAtom.dir/src/DetectorConstruction.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/DetectorConstruction.cc -o CMakeFiles/sim_MuonicAtom.dir/src/DetectorConstruction.cc.s

CMakeFiles/sim_MuonicAtom.dir/src/EventAction.cc.o: CMakeFiles/sim_MuonicAtom.dir/flags.make
CMakeFiles/sim_MuonicAtom.dir/src/EventAction.cc.o: /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/EventAction.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/mu_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/sim_MuonicAtom.dir/src/EventAction.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sim_MuonicAtom.dir/src/EventAction.cc.o -c /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/EventAction.cc

CMakeFiles/sim_MuonicAtom.dir/src/EventAction.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sim_MuonicAtom.dir/src/EventAction.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/EventAction.cc > CMakeFiles/sim_MuonicAtom.dir/src/EventAction.cc.i

CMakeFiles/sim_MuonicAtom.dir/src/EventAction.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sim_MuonicAtom.dir/src/EventAction.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/EventAction.cc -o CMakeFiles/sim_MuonicAtom.dir/src/EventAction.cc.s

CMakeFiles/sim_MuonicAtom.dir/src/PhysicsList.cc.o: CMakeFiles/sim_MuonicAtom.dir/flags.make
CMakeFiles/sim_MuonicAtom.dir/src/PhysicsList.cc.o: /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/PhysicsList.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/mu_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/sim_MuonicAtom.dir/src/PhysicsList.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sim_MuonicAtom.dir/src/PhysicsList.cc.o -c /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/PhysicsList.cc

CMakeFiles/sim_MuonicAtom.dir/src/PhysicsList.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sim_MuonicAtom.dir/src/PhysicsList.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/PhysicsList.cc > CMakeFiles/sim_MuonicAtom.dir/src/PhysicsList.cc.i

CMakeFiles/sim_MuonicAtom.dir/src/PhysicsList.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sim_MuonicAtom.dir/src/PhysicsList.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/PhysicsList.cc -o CMakeFiles/sim_MuonicAtom.dir/src/PhysicsList.cc.s

CMakeFiles/sim_MuonicAtom.dir/src/PrimaryGeneratorAction.cc.o: CMakeFiles/sim_MuonicAtom.dir/flags.make
CMakeFiles/sim_MuonicAtom.dir/src/PrimaryGeneratorAction.cc.o: /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/PrimaryGeneratorAction.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/mu_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/sim_MuonicAtom.dir/src/PrimaryGeneratorAction.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sim_MuonicAtom.dir/src/PrimaryGeneratorAction.cc.o -c /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/PrimaryGeneratorAction.cc

CMakeFiles/sim_MuonicAtom.dir/src/PrimaryGeneratorAction.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sim_MuonicAtom.dir/src/PrimaryGeneratorAction.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/PrimaryGeneratorAction.cc > CMakeFiles/sim_MuonicAtom.dir/src/PrimaryGeneratorAction.cc.i

CMakeFiles/sim_MuonicAtom.dir/src/PrimaryGeneratorAction.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sim_MuonicAtom.dir/src/PrimaryGeneratorAction.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/PrimaryGeneratorAction.cc -o CMakeFiles/sim_MuonicAtom.dir/src/PrimaryGeneratorAction.cc.s

CMakeFiles/sim_MuonicAtom.dir/src/RootOutput.cc.o: CMakeFiles/sim_MuonicAtom.dir/flags.make
CMakeFiles/sim_MuonicAtom.dir/src/RootOutput.cc.o: /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/RootOutput.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/mu_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/sim_MuonicAtom.dir/src/RootOutput.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sim_MuonicAtom.dir/src/RootOutput.cc.o -c /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/RootOutput.cc

CMakeFiles/sim_MuonicAtom.dir/src/RootOutput.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sim_MuonicAtom.dir/src/RootOutput.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/RootOutput.cc > CMakeFiles/sim_MuonicAtom.dir/src/RootOutput.cc.i

CMakeFiles/sim_MuonicAtom.dir/src/RootOutput.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sim_MuonicAtom.dir/src/RootOutput.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/RootOutput.cc -o CMakeFiles/sim_MuonicAtom.dir/src/RootOutput.cc.s

CMakeFiles/sim_MuonicAtom.dir/src/RunAction.cc.o: CMakeFiles/sim_MuonicAtom.dir/flags.make
CMakeFiles/sim_MuonicAtom.dir/src/RunAction.cc.o: /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/RunAction.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/mu_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/sim_MuonicAtom.dir/src/RunAction.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sim_MuonicAtom.dir/src/RunAction.cc.o -c /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/RunAction.cc

CMakeFiles/sim_MuonicAtom.dir/src/RunAction.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sim_MuonicAtom.dir/src/RunAction.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/RunAction.cc > CMakeFiles/sim_MuonicAtom.dir/src/RunAction.cc.i

CMakeFiles/sim_MuonicAtom.dir/src/RunAction.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sim_MuonicAtom.dir/src/RunAction.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/RunAction.cc -o CMakeFiles/sim_MuonicAtom.dir/src/RunAction.cc.s

CMakeFiles/sim_MuonicAtom.dir/src/SteppingAction.cc.o: CMakeFiles/sim_MuonicAtom.dir/flags.make
CMakeFiles/sim_MuonicAtom.dir/src/SteppingAction.cc.o: /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/SteppingAction.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/mu_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/sim_MuonicAtom.dir/src/SteppingAction.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sim_MuonicAtom.dir/src/SteppingAction.cc.o -c /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/SteppingAction.cc

CMakeFiles/sim_MuonicAtom.dir/src/SteppingAction.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sim_MuonicAtom.dir/src/SteppingAction.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/SteppingAction.cc > CMakeFiles/sim_MuonicAtom.dir/src/SteppingAction.cc.i

CMakeFiles/sim_MuonicAtom.dir/src/SteppingAction.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sim_MuonicAtom.dir/src/SteppingAction.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim/src/SteppingAction.cc -o CMakeFiles/sim_MuonicAtom.dir/src/SteppingAction.cc.s

# Object files for target sim_MuonicAtom
sim_MuonicAtom_OBJECTS = \
"CMakeFiles/sim_MuonicAtom.dir/MuonicAtom.cc.o" \
"CMakeFiles/sim_MuonicAtom.dir/src/ActionInitialization.cc.o" \
"CMakeFiles/sim_MuonicAtom.dir/src/DetectorConstruction.cc.o" \
"CMakeFiles/sim_MuonicAtom.dir/src/EventAction.cc.o" \
"CMakeFiles/sim_MuonicAtom.dir/src/PhysicsList.cc.o" \
"CMakeFiles/sim_MuonicAtom.dir/src/PrimaryGeneratorAction.cc.o" \
"CMakeFiles/sim_MuonicAtom.dir/src/RootOutput.cc.o" \
"CMakeFiles/sim_MuonicAtom.dir/src/RunAction.cc.o" \
"CMakeFiles/sim_MuonicAtom.dir/src/SteppingAction.cc.o"

# External object files for target sim_MuonicAtom
sim_MuonicAtom_EXTERNAL_OBJECTS =

sim_MuonicAtom: CMakeFiles/sim_MuonicAtom.dir/MuonicAtom.cc.o
sim_MuonicAtom: CMakeFiles/sim_MuonicAtom.dir/src/ActionInitialization.cc.o
sim_MuonicAtom: CMakeFiles/sim_MuonicAtom.dir/src/DetectorConstruction.cc.o
sim_MuonicAtom: CMakeFiles/sim_MuonicAtom.dir/src/EventAction.cc.o
sim_MuonicAtom: CMakeFiles/sim_MuonicAtom.dir/src/PhysicsList.cc.o
sim_MuonicAtom: CMakeFiles/sim_MuonicAtom.dir/src/PrimaryGeneratorAction.cc.o
sim_MuonicAtom: CMakeFiles/sim_MuonicAtom.dir/src/RootOutput.cc.o
sim_MuonicAtom: CMakeFiles/sim_MuonicAtom.dir/src/RunAction.cc.o
sim_MuonicAtom: CMakeFiles/sim_MuonicAtom.dir/src/SteppingAction.cc.o
sim_MuonicAtom: CMakeFiles/sim_MuonicAtom.dir/build.make
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4Tree.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4GMocren.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4visHepRep.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4RayTracer.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4VRML.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4OpenGL.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4gl2ps.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4interfaces.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4persistency.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4error_propagation.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4readout.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4physicslists.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4parmodels.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4FR.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4vis_management.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4modeling.dylib
sim_MuonicAtom: /usr/local/opt/qt/lib/QtOpenGL.framework/QtOpenGL
sim_MuonicAtom: /usr/local/opt/qt/lib/QtPrintSupport.framework/QtPrintSupport
sim_MuonicAtom: /usr/local/opt/qt/lib/QtWidgets.framework/QtWidgets
sim_MuonicAtom: /usr/local/opt/qt/lib/QtGui.framework/QtGui
sim_MuonicAtom: /usr/local/opt/qt/lib/QtCore.framework/QtCore
sim_MuonicAtom: /usr/local/lib/libxerces-c.so
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4run.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4event.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4tracking.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4processes.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4analysis.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4zlib.dylib
sim_MuonicAtom: /usr/lib/libexpat.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4digits_hits.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4track.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4particles.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4geometry.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4materials.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4graphics_reps.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4intercoms.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4global.dylib
sim_MuonicAtom: /usr/local/Cellar/geant4/10.5.1/lib/libG4clhep.dylib
sim_MuonicAtom: CMakeFiles/sim_MuonicAtom.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/mu_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable sim_MuonicAtom"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sim_MuonicAtom.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sim_MuonicAtom.dir/build: sim_MuonicAtom

.PHONY : CMakeFiles/sim_MuonicAtom.dir/build

CMakeFiles/sim_MuonicAtom.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sim_MuonicAtom.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sim_MuonicAtom.dir/clean

CMakeFiles/sim_MuonicAtom.dir/depend:
	cd /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/mu_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/MuonicAtom_sim /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/mu_build /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/mu_build /Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/mu_build/CMakeFiles/sim_MuonicAtom.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sim_MuonicAtom.dir/depend

