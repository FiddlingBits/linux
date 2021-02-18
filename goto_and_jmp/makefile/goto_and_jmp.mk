####################################################################################################
# FILE:	 goto_and_jmp.mk
# BRIEF: goto And *jmp Makefile
####################################################################################################

####################################################################################################
# Targets
####################################################################################################

BUILD_DIRECTORY := ../build
EXECUTABLE := goto_and_jmp.exe

####################################################################################################
# Defines
####################################################################################################

APPLICATION_DEFINES := 

####################################################################################################
# Include Paths
####################################################################################################

APPLICATION_INCLUDE_PATHS := \
  -I../../common/

####################################################################################################
# Compiler Flags
####################################################################################################

APPLICATION_COMPILER_FLAGS := \
  -Weverything                \
  -Wno-missing-noreturn       \
  -Wno-unreachable-code       \
  -Wno-unused-parameter

####################################################################################################
# Sources
####################################################################################################

APPLICATION_SOURCES := \
  ../goto_and_jmp.c

####################################################################################################
# Rules
####################################################################################################

all: $(BUILD_DIRECTORY)
	@echo "Building Executable: $(EXECUTABLE)"
	@clang -o $(EXECUTABLE) $(APPLICATION_DEFINES) $(APPLICATION_COMPILER_FLAGS) $(APPLICATION_INCLUDE_PATHS) $(APPLICATION_SOURCES)
	@mv $(EXECUTABLE) $(BUILD_DIRECTORY)
	@rm -f *.o

$(BUILD_DIRECTORY):
	@mkdir $(BUILD_DIRECTORY)

clean: FORCE
	@rm -fr $(BUILD_DIRECTORY)

FORCE: