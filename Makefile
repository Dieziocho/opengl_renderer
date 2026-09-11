SOURCE_DIR := source
INCLUDE_DIR := source/include
BUILD_DIR := build
TARGET_DIR := target

TARGET := target/output

.PHONY: all build run
all: build run
build:
	$(MAKE) -j$(nproc) $(TARGET)

run: $(TARGET)
	./$(TARGET)

clear:
	@rm -rf $(BUILD_DIR) target

CXX := ccache g++

CXX_FLAGS := \
	-g -O0 \
	-Wall -Wextra -pedantic \
	-std=c++26 \
	-MMD -MP \
	-I $(INCLUDE_DIR) 

LD_FLAGS := \
    -lglfw \
    -lGL \
    -lX11 \
    -lpthread \
    -lXrandr \
    -lXi \
    -ldl \
    -lassimp


SRC_FILES := $(shell find "$(SOURCE_DIR)" \( -name "*.cpp" -o -name "*.c" \) ! -path "$(INCLUDE_DIR)/*")
OBJ_FILES := $(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(filter %.cpp,$(SRC_FILES)))
OBJ_FILES += $(patsubst $(SOURCE_DIR)/%.c,$(BUILD_DIR)/%.o,$(filter %.c,$(SRC_FILES)))
DEP_FILES := $(OBJ_FILES:.o=.d)

$(TARGET): $(OBJ_FILES)
	@mkdir -p $(dir $@)
	$(CXX) -o $@ $^ $(LD_FLAGS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) -c $< -o $@
	
-include $(DEP_FILES)
