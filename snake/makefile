CXX = g++
CFLAGS = -std=c++17 -Wall -pedantic -g

BIN_NAME = snake

SRC_DIR = src

DEST_DIR = bin

OBJS := $(patsubst $(SRC_DIR)/%.cc, $(DEST_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cc))

TARGET := $(DEST_DIR)/$(BIN_NAME)

$(DEST_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) -c $< -o $@ $(CFLAGS)

all: $(BIN_NAME)

$(BIN_NAME): dirs $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(CFLAGS) -lcurses

.PHONY: dirs
dirs:
	mkdir -p $(DEST_DIR)

.PHONY: clean
clean:
	-rm $(TARGET) $(OBJS)
