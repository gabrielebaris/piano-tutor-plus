# ------------------------------------------------
# PianoTutor+ Makefile
# ------------------------------------------------

# Project name (also final executable name)
TARGET   = pianotutor+

# Folder organization
INCDIR = inc
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# External dependencies
LED_STRIP_LIB_FOLDER = /home/pi/rpi_ws281x
LED_STRIP_LIB_NAME = ws2811

# Toolchain and flags
CC		= g++
CFLAGS	= -Wall -pedantic -I./$(INCDIR) -I$(LED_STRIP_LIB_FOLDER)
LINKER	= g++
LFLAGS	= -Wall -I./$(INCDIR) -lm -L$(LED_STRIP_LIB_FOLDER) -l$(LED_STRIP_LIB_NAME) -lasound


# Files and macros
SOURCES		:= $(wildcard $(SRCDIR)/*.cpp)
INCLUDES	:= $(wildcard $(INCDIR)/*.h)
OBJECTS		:= $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
rm 			= rm -f
mkdir		= mkdir -p


# Make rules
.PHONY: all
all: directories $(BINDIR)/$(TARGET)

.PHONY: debug
debug: CFLAGS += -DDEBUG -g -O0
debug: directories $(BINDIR)/$(TARGET)


$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo $<" compiled successfully"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed"

.PHONY: directories
directories:
	@$(mkdir) $(OBJDIR) $(BINDIR)