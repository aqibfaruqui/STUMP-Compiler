CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
OUTPUT_DIR = output
SAMPLES_DIR = samples

SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/lexer.cpp $(SRC_DIR)/parser.cpp $(SRC_DIR)/generator.cpp
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET = $(BIN_DIR)/stump

# Default target
all: $(TARGET)

# Create directories
$(BUILD_DIR) $(BIN_DIR) $(OUTPUT_DIR):
	mkdir -p $@

# Build executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Test with sample file
test: $(TARGET) | $(OUTPUT_DIR)
	./$(TARGET) $(SAMPLES_DIR)/test.stump

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(OUTPUT_DIR)

.PHONY: all debug release test clean install