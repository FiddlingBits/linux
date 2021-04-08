####################################################################################################
# FILE:	 nonreentrant.mk
# BRIEF: Non-Reentrant Makefile
####################################################################################################

####################################################################################################
# Targets
####################################################################################################

BUILD_DIRECTORY := ../build
EXECUTABLE := nonreentrant.exe
LIBRARY := common.a

####################################################################################################
# Defines
####################################################################################################

APPLICATION_DEFINES := 

LIBRARY_DEFINES := 

####################################################################################################
# Include Paths
####################################################################################################

APPLICATION_INCLUDE_PATHS := \
  -I../../common/

LIBRARY_INCLUDE_PATHS := \
  -I../../common/

####################################################################################################
# Compiler Flags
####################################################################################################

APPLICATION_COMPILER_FLAGS :=   \
  -Weverything                  \
  -Wno-disabled-macro-expansion \
  -Wno-unreachable-code         \
  -Wno-unused-parameter

LIBRARY_COMPILER_FLAGS := \
  -Weverything            \
  -Wno-format-nonliteral

####################################################################################################
# Sources
####################################################################################################

APPLICATION_SOURCES := \
  ../nonreentrant.c

LIBRARY_SOURCES :=      \
  ../../common/error.c  \
  ../../common/number.c

####################################################################################################
# Rules
####################################################################################################

all: $(BUILD_DIRECTORY)/$(LIBRARY)
	@echo "Building Executable: $(EXECUTABLE)"
	@clang -c $(APPLICATION_DEFINES) $(APPLICATION_COMPILER_FLAGS) $(APPLICATION_INCLUDE_PATHS) $(APPLICATION_SOURCES)
	@clang *.o -o $(EXECUTABLE) $(BUILD_DIRECTORY)/$(LIBRARY) -lcrypt
	@mv $(EXECUTABLE) $(BUILD_DIRECTORY)
	@rm -f *.o

$(BUILD_DIRECTORY)/$(LIBRARY): $(BUILD_DIRECTORY)
	@clang -c $(LIBRARY_DEFINES) $(LIBRARY_COMPILER_FLAGS) $(LIBRARY_INCLUDE_PATHS) $(LIBRARY_SOURCES)
	@ar rcs $(LIBRARY) *.o
	@mv $(LIBRARY) $(BUILD_DIRECTORY)
	@rm -f *.o

$(BUILD_DIRECTORY):
	@mkdir $(BUILD_DIRECTORY)

clean: FORCE
	@rm -fr $(BUILD_DIRECTORY)

FORCE:
