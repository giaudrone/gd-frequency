# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++23 -Wall

# Source files
SRCS = src/frequency.cpp src/main.cpp

# Output file
OUT = gd-frequency

# Target to compile the program
$(OUT): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(OUT)

# Clean up generated files
clean:
	rm -f $(OUT)

# Phony targets
.PHONY: clean

