# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++23 -Wall

# Source files
SRCS = src/frequency.cpp src/main.cpp

# Output file
OUT = gd-frequency
BUILD_DIR = build
PREFIX = /usr
BIN_DIR = $(PREFIX)/bin
SHARE_DIR = $(PREFIX)/share/gd-frequency

# Target to compile the program
all: $(OUT) 

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OUT): $(BUILD_DIR) $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(BUILD_DIR)/$(OUT)

install: $(OUT)
	# share folder for dictionary and bin
	sudo mkdir -p $(SHARE_DIR)
	sudo install -Dm644 ./data/jpdb.json $(SHARE_DIR)/jpdb.json

	sudo install -Dm755 $(BUILD_DIR)/$(OUT) $(BIN_DIR)/$(OUT)

clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean install

