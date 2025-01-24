# Compiler and flags
CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -g -O3
LDFLAGS = -lSDL2 -lSDL2_image -lm

# Directories
SRC_DIR = src
BUILD_DIR = build
ASSETS_DIR = assets

# Subdirectories under SRC_DIR
CONTROLLER_DIR = $(SRC_DIR)/Controller
MODELS_DIR = $(SRC_DIR)/Models
VIEW_DIR = $(SRC_DIR)/View

# Source files
SRCS = $(CONTROLLER_DIR)/aquaEngine.cpp \
       $(SRC_DIR)/main.cpp \
       $(MODELS_DIR)/Cluster.cpp \
       $(MODELS_DIR)/Creature.cpp \
       $(MODELS_DIR)/Fish.cpp \
       $(MODELS_DIR)/Predator.cpp \
       $(VIEW_DIR)/CurvedGeometry.cpp \
       $(VIEW_DIR)/Render.cpp

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Target executable
TARGET = $(BUILD_DIR)/FishEngine

# Default target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean
