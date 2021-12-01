NAME = nvimim_2021
VERSION = 2021
OUTPUT = nvimim

# Directory for the object files
BUILD = ./build

# Compiler for the language C
CC = clang

# Compiler for the language C++
CXXC = clang++

# Compiler for the language Objective C
OBJCC = clang

# Linker
LINKER = clang++

# Compiler and linker flags
FLAGS =

# Compiler flags for language C
CFLAGS = $(FLAGS) -I./include

# Compiler flags for language C++
CXXFLAGS = $(FLAGS) -I./include -DME_DEBUG -g -Wall -Wextra -std=c++20

# Compiler flags for language Objective C
OBJCFLAGS = $(FLAGS) -I./include

# Linker flags
LFLAGS = $(FLAGS)

SOURCES = \
	./src/nvimim/Main.cpp \
	./src/nvimim/Listener.cpp \
	./src/nvim/Nvim.cpp \
	./src/nvim/Type.cpp \
	./src/nvim/types/Buffer.cpp \
	./src/mpack/mpack.c
OBJECTS = $(SOURCES:%=$(BUILD)/%.o)
DEPENDS = $(SOURCES:%=$(BUILD)/%.d)

EXTERNALS =

.PHONY: $(NAME)
$(NAME): $(OUTPUT) $(EXTERNALS)

$(OUTPUT): $(OBJECTS)
	@$(LINKER) -o $@ $^ $(LFLAGS)

-include $(DEPENDS)

$(BUILD)/./src/nvimim/Main.cpp.o: ./src/nvimim/Main.cpp
	@mkdir -p $(dir $@)
	@echo "Building C++ source '$<'"
	@$(CXXC) -c -o $@ $< $(CXXFLAGS) -MMD

$(BUILD)/./src/nvimim/Listener.cpp.o: ./src/nvimim/Listener.cpp
	@mkdir -p $(dir $@)
	@echo "Building C++ source '$<'"
	@$(CXXC) -c -o $@ $< $(CXXFLAGS) -MMD

$(BUILD)/./src/nvim/Nvim.cpp.o: ./src/nvim/Nvim.cpp
	@mkdir -p $(dir $@)
	@echo "Building C++ source '$<'"
	@$(CXXC) -c -o $@ $< $(CXXFLAGS) -MMD

$(BUILD)/./src/nvim/Type.cpp.o: ./src/nvim/Type.cpp
	@mkdir -p $(dir $@)
	@echo "Building C++ source '$<'"
	@$(CXXC) -c -o $@ $< $(CXXFLAGS) -MMD

$(BUILD)/./src/nvim/types/Buffer.cpp.o: ./src/nvim/types/Buffer.cpp
	@mkdir -p $(dir $@)
	@echo "Building C++ source '$<'"
	@$(CXXC) -c -o $@ $< $(CXXFLAGS) -MMD

$(BUILD)/./src/mpack/mpack.c.o: ./src/mpack/mpack.c
	@mkdir -p $(dir $@)
	@echo "Building C source '$<'"
	@$(CC) -c -o $@ $< $(CFLAGS) -MMD

.PHONY: install
install:

.PHONY: clean
clean:
	rm -rf $(OUTPUT) $(OBJECTS) $(DEPENDS)
