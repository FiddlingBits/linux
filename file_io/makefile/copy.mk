####################################################################################################
# FILE:	 copy.mk
# BRIEF: Copy Makefile
####################################################################################################

####################################################################################################
# Targets
####################################################################################################

BUILD_DIRECTORY := ../build
EXECUTABLE := copy.exe
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

APPLICATION_COMPILER_FLAGS := \
  -Weverything

LIBRARY_COMPILER_FLAGS := \
  -Weverything            \
  -Wno-format-nonliteral

####################################################################################################
# Sources
####################################################################################################

APPLICATION_SOURCES := \
  ../copy.c

LIBRARY_SOURCES :=      \
  ../../common/error.c  \
  ../../common/number.c

####################################################################################################
# Rules
####################################################################################################

all: $(BUILD_DIRECTORY)/$(LIBRARY)
	@echo "Building Executable: $(EXECUTABLE)"
	@clang -c $(APPLICATION_DEFINES) $(APPLICATION_COMPILER_FLAGS) $(APPLICATION_INCLUDE_PATHS) $(APPLICATION_SOURCES)
	@clang *.o -o $(EXECUTABLE) $(BUILD_DIRECTORY)/$(LIBRARY)
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
