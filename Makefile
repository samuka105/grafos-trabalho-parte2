# Compiler
CXX := g++

# Directories
SRC_DIR := src
INC_DIR := include

# Files
SRCS := $(wildcard $(SRC_DIR)/*.cpp) main.cpp
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(SRC_DIR)/%.o,$(filter-out main.cpp,$(SRCS)))
MAIN_OBJ := main.o
DEPS := $(wildcard $(INC_DIR)/*.hpp)

# Compiler flags
CXXFLAGS := -std=c++11 -Wall -Wextra

# Output executable
TARGET := graph_project

.PHONY: all clean

all: $(TARGET)

# Link objects to create the executable
$(TARGET): $(OBJS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile the .cpp files in src to .o files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Compile main.cpp to main.o
$(MAIN_OBJ): main.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Clean generated files
clean:
	@rm -f $(OBJS) $(MAIN_OBJ) $(TARGET)
