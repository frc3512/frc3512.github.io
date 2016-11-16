# The following targets are supported by this makefile. Indentations represent
# dependencies.
#
# all
#   debug
#   release
# clean
#   clean-debug
#   clean-release

NAME := example

# Compiler prefix for cross compiling on non-Windows hosts if needed
# (e.g. x86_64-w64-mingw32-)
PREFIX :=

CC := gcc
CFLAGS_DEBUG := -O0 -g3 -Wall -std=c11
CFLAGS_RELEASE := -O3 -Wall -s -std=c11 -flto

CXX := g++
CXXFLAGS_DEBUG := -O0 -g3 -Wall -std=c++1y
CXXFLAGS_RELEASE := -O3 -Wall -s -std=c++1y -flto

# Specify defines with -D directives here
DEFINES_DEBUG :=
DEFINES_RELEASE :=

LD := g++

# Platform specific variables
ifeq ($(OS), Windows_NT)
	# Specify Windows include paths with -I directives here
	IFLAGS :=

	# Specify Windows libs with -l directives here
	LDFLAGS := -pthread

	# Assign executable name
	EXEC := $(NAME).exe
else
	# Specify Linux include paths with -I directives here
	IFLAGS :=

	# Specify Linux libs with -l directives here
	LDFLAGS := -pthread

ifeq ($(strip $(PREFIX)),)
	# Assign executable name
	EXEC := $(NAME)
else
	# Assign executable name with .exe extension if using a cross compiler
	EXEC := $(NAME).exe
endif

	# Prepend optional prefix
	CC := $(PREFIX)$(strip $(CC))
	CXX := $(PREFIX)$(strip $(CXX))
	LD := $(PREFIX)$(strip $(LD))
endif

SRCDIR := src

# Make does not offer a recursive wildcard function, so here's one:
rwildcard=$(wildcard $1$2) $(foreach dir,$(wildcard $1*),$(call rwildcard,$(dir)/,$2))

# Recursively find all C source files
SRC_C := $(call rwildcard,$(SRCDIR)/,*.c)

# Recursively find all C++ source files
SRC_CXX := $(call rwildcard,$(SRCDIR)/,*.cpp)

# Create raw list of object files
C_OBJ := $(SRC_C:.c=.o)
CXX_OBJ := $(SRC_CXX:.cpp=.o)

# Create list of object files for debug build type
OBJDIR_DEBUG := Debug
C_OBJ_DEBUG := $(addprefix $(OBJDIR_DEBUG)/,$(C_OBJ))
CXX_OBJ_DEBUG := $(addprefix $(OBJDIR_DEBUG)/,$(CXX_OBJ))

# Create list of object files for release build type
OBJDIR_RELEASE := Release
C_OBJ_RELEASE := $(addprefix $(OBJDIR_RELEASE)/,$(C_OBJ))
CXX_OBJ_RELEASE := $(addprefix $(OBJDIR_RELEASE)/,$(CXX_OBJ))

.PHONY: all
all: debug release

# Define a string comparison function: "String EQual"
seq = $(and $(findstring $1,$2),$(findstring $2,$1))

# Define function for determining if given target is within a list of targets
# (If $1 is an element of $2)
# Returns a list whose size equals the number of occurences of $1 in $2
targetelem = $(foreach goal,$2, \
$(if $(call seq, $1 , $(goal) ),found,))

# Determine if any of the clean targets are about to be built
# If 'clean' won't be built
ifeq (,$(strip $(call targetelem,clean,$(MAKECMDGOALS))))

# If 'clean-debug' won't be built and either 'all' or 'debug' will be, generate
# the dependency files (no use in regenerating the dependencies if the target
# they are for won't be built).
ifeq (,$(strip $(call targetelem,clean-debug,$(MAKECMDGOALS))))
ifneq (,$(strip $(call targetelem,all,$(MAKECMDGOALS)) $(call targetelem,debug,$(MAKECMDGOALS))))
-include $(C_OBJ_DEBUG:.o=.d) $(CXX_OBJ_DEBUG:.o=.d)
# If no targets were specified, regenerate the dependencies
else ifeq (,$(strip $(MAKECMDGOALS)))
-include $(C_OBJ_DEBUG:.o=.d) $(CXX_OBJ_DEBUG:.o=.d)
endif
endif

# If 'clean-release' won't be built and either 'all' or 'release' will be,
# generate the dependency files (no use in regenerating the dependencies if the
# target they are for won't be built).
ifeq (,$(strip $(call targetelem,clean-release,$(MAKECMDGOALS))))
ifneq (,$(strip $(call targetelem,all,$(MAKECMDGOALS)) $(call targetelem,release,$(MAKECMDGOALS))))
-include $(C_OBJ_RELEASE:.o=.d) $(CXX_OBJ_RELEASE:.o=.d)
# If no targets were specified, regenerate the dependencies
else ifeq (,$(strip $(MAKECMDGOALS)))
-include $(C_OBJ_RELEASE:.o=.d) $(CXX_OBJ_RELEASE:.o=.d)
endif
endif

endif

.PHONY: debug
debug: $(OBJDIR_DEBUG)/$(EXEC)

$(OBJDIR_DEBUG)/$(EXEC): $(C_OBJ_DEBUG) $(CXX_OBJ_DEBUG)
	@mkdir -p $(@D)
	@echo Linking $@
ifdef VERBOSE
	$(LD) -o $@ $(C_OBJ_DEBUG) $(CXX_OBJ_DEBUG) $(LDFLAGS)
else
	@$(LD) -o $@ $(C_OBJ_DEBUG) $(CXX_OBJ_DEBUG) $(LDFLAGS)
endif

# Pattern rule for building object file from C source
# The -MMD flag generates .d files to track changes in header files included in
# the source.
$(C_OBJ_DEBUG): $(OBJDIR_DEBUG)/%.o: %.c
	@mkdir -p $(@D)
	@echo Building C object $@
ifdef VERBOSE
	$(CC) $(CFLAGS_DEBUG) $(DEFINES_DEBUG) $(IFLAGS) -MMD -c -o $@ $<
else
	@$(CC) $(CFLAGS_DEBUG) $(DEFINES_DEBUG) $(IFLAGS) -MMD -c -o $@ $<
endif

# Pattern rule for building object file from C++ source
# The -MMD flag generates .d files to track changes in header files included in
# the source.
$(CXX_OBJ_DEBUG): $(OBJDIR_DEBUG)/%.o: %.cpp
	@mkdir -p $(@D)
	@echo Building CXX object $@
ifdef VERBOSE
	$(CXX) $(CXXFLAGS_DEBUG) $(DEFINES_DEBUG) $(IFLAGS) -MMD -c -o $@ $<
else
	@$(CXX) $(CXXFLAGS_DEBUG) $(DEFINES_DEBUG) $(IFLAGS) -MMD -c -o $@ $<
endif

.PHONY: release
release: $(OBJDIR_RELEASE)/$(EXEC)

$(OBJDIR_RELEASE)/$(EXEC): $(C_OBJ_RELEASE) $(CXX_OBJ_RELEASE)
	@mkdir -p $(@D)
	@echo Linking $@
ifdef VERBOSE
	$(LD) -o $@ $(C_OBJ_RELEASE) $(CXX_OBJ_RELEASE) $(LDFLAGS)
else
	@$(LD) -o $@ $(C_OBJ_RELEASE) $(CXX_OBJ_RELEASE) $(LDFLAGS)
endif

# Pattern rule for building object file from C source
# The -MMD flag generates .d files to track changes in header files included in
# the source.
$(C_OBJ_RELEASE): $(OBJDIR_RELEASE)/%.o: %.c
	@mkdir -p $(@D)
	@echo Building C object $@
ifdef VERBOSE
	$(CC) $(CFLAGS_RELEASE) $(DEFINES_RELEASE) $(IFLAGS) -MMD -c -o $@ $<
else
	@$(CC) $(CFLAGS_RELEASE) $(DEFINES_RELEASE) $(IFLAGS) -MMD -c -o $@ $<
endif

# Pattern rule for building object file from C++ source
# The -MMD flag generates .d files to track changes in header files included in
# the source.
$(CXX_OBJ_RELEASE): $(OBJDIR_RELEASE)/%.o: %.cpp
	@mkdir -p $(@D)
	@echo Building CXX object $@
ifdef VERBOSE
	$(CXX) $(CXXFLAGS_RELEASE) $(DEFINES_RELEASE) $(IFLAGS) -MMD -c -o $@ $<
else
	@$(CXX) $(CXXFLAGS_RELEASE) $(DEFINES_RELEASE) $(IFLAGS) -MMD -c -o $@ $<
endif

# Cleans everything
.PHONY: clean
clean: clean-debug clean-release

# Cleans the debug build directory
.PHONY: clean-debug
clean-debug:
	@echo Removing Debug object files
ifdef VERBOSE
	-$(RM) -r $(OBJDIR_DEBUG)/$(SRCDIR)
	-$(RM) $(OBJDIR_DEBUG)/$(EXEC)
else
	-@$(RM) -r $(OBJDIR_DEBUG)/$(SRCDIR)
	-@$(RM) $(OBJDIR_DEBUG)/$(EXEC)
endif

# Cleans the release build directory
.PHONY: clean-release
clean-release:
	@echo Removing Release object files
ifdef VERBOSE
	-$(RM) -r $(OBJDIR_RELEASE)/$(SRCDIR)
	-$(RM) $(OBJDIR_RELEASE)/$(EXEC)
else
	-@$(RM) -r $(OBJDIR_RELEASE)/$(SRCDIR)
	-@$(RM) $(OBJDIR_RELEASE)/$(EXEC)
endif
