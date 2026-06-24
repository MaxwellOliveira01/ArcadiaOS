CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Isrc
TARGET := dispatcher

SRC_DIR := src
BUILD_DIR := build

SRCS := $(SRC_DIR)/main.cpp \
	$(SRC_DIR)/utils/input_parser.cpp
	
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) processes.txt files.txt string.txt

clean:
	rm -rf $(BUILD_DIR) $(TARGET)