# Compiler and flags
CXX = g++-14.2.0
CXXFLAGS = -std=c++20 -fmodules-ts -Wall -g

# Files
ORDER_FILE = order.txt
EXEC = watopoly

# Read files from order.txt
SRC := $(shell cat $(ORDER_FILE))
OBJ := $(SRC:.cc=.o)
DEP := $(OBJ:.o=.d)

# Default target
$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile object files from .cc files in correct order
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include dependency files
-include $(DEP)

# Clean build artifacts
.PHONY: clean
clean:
	rm -f $(OBJ) $(DEP) $(EXEC)
