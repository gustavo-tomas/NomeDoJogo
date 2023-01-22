CXX=g++

CFLAGS= -g -std=c++11 -Wall -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

TARGET ?= Run

BUILD_DIR ?= build
SRC_DIRS ?= src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

INC_DIRS := header
INC_FLAGS := $(addprefix -I , $(INC_DIRS))

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(CFLAGS) -o $@
	@echo "\n\nNomeDaEngine compiled successfully."
	@echo "Run with ./Run"

$(BUILD_DIR)/%.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR) $(TARGET)

MKDIR_P ?= mkdir -p