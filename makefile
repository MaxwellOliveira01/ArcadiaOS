CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Isrc
TARGET := dispatcher

SRC_DIR := src
BUILD_DIR := build

SRCS := $(SRC_DIR)/main.cpp \
	$(SRC_DIR)/utils/input_parser.cpp \
	$(SRC_DIR)/io/resource_manager.cpp \
	$(SRC_DIR)/dispatcher/dispatcher.cpp \
	$(SRC_DIR)/scheduler/scheduler.cpp \
	$(SRC_DIR)/scheduler/real_time_queue.cpp \
	$(SRC_DIR)/scheduler/user_queue.cpp \
	$(SRC_DIR)/scheduler/global_queue.cpp \
	$(SRC_DIR)/process/process.cpp \
	$(SRC_DIR)/filesystem/file_system.cpp \

	
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) testcases/processes.txt testcases/files.txt testcases/string.txt

clean:
	rm -rf $(BUILD_DIR) $(TARGET)