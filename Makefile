# Compiler
CXX := g++

# Directories
SRC_DIR := src
INC_DIR := include

# Files
SRCS := $(wildcard $(SRC_DIR)/*.cpp) main.cpp
OBJS := $(patsubst %.cpp, %.o, $(SRCS))

# Compiler flags
CXXFLAGS := -std=c++11 -Wall -Wextra

# Output executable
TARGET := graph_project

.PHONY: all clean

all: $(TARGET)

# Link objects to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

# Compile each .cpp file to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Clean generated files
clean:
	@rm -f $(OBJS) $(TARGET)
