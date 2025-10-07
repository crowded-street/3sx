PLATFORM ?=

# Binaries

ifeq ($(PLATFORM),windows)
	MAIN := sf33rd.exe
else
	MAIN := sf33rd
endif

# Directories

BUILD_DIR := build/$(PLATFORM)
SRC_DIR := src
INCLUDE_DIR := include

# Flags

COMMON_INCLUDES := -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/sdk -I$(INCLUDE_DIR)/cri -I$(INCLUDE_DIR)/cri/ee -Izlib

CLANG_WARNINGS := -Wall -Werror
CLANG_WARNINGS += -Wpointer-to-int-cast
CLANG_WARNINGS += -Wno-macro-redefined -Wno-deprecated-non-prototype
CLANG_WARNINGS += -Wno-missing-braces -Wno-self-assign -Wno-format
CLANG_WARNINGS += -Wno-tautological-constant-out-of-range-compare -Wno-tautological-overlap-compare
CLANG_WARNINGS += -Wno-gcc-compat -Wno-unknown-pragmas -Wno-c2x-extensions
CLANG_WARNINGS += -Wno-unused-value -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-comparison -Wno-unused-function

# These should be enabled in the future
CLANG_WARNINGS += -Wno-int-to-pointer-cast
CLANG_WARNINGS += -Wno-int-conversion
CLANG_WARNINGS += -Wno-incompatible-pointer-types
CLANG_WARNINGS += -Wno-incompatible-function-pointer-types
CLANG_WARNINGS += -Wno-pointer-sign
CLANG_WARNINGS += -Wno-shift-count-overflow

CLANG_DEFINES := -DTARGET_SDL3 -DSOUND_DISABLED -DXPT_TGT_EE -D_POSIX_C_SOURCE -DDEBUG
CLANG_INCLUDES := $(COMMON_INCLUDES) -Ilibco
CLANG_FLAGS := $(CLANG_INCLUDES) $(CLANG_WARNINGS) $(CLANG_DEFINES) -std=gnu11 -O0
CLANG_LINKER_FLAGS := -lm -g -Llibco/build -llibco
PORT_PACKAGES := sdl3 libavcodec libavutil libavformat libswresample
CLANG_FLAGS += $(shell pkg-config --cflags $(PORT_PACKAGES))
CLANG_LINKER_FLAGS += $(shell pkg-config --libs $(PORT_PACKAGES))

ifeq ($(PLATFORM),windows)
	CLANG_LINKER_FLAGS += --for-linker --pdb=$(BUILD_DIR)/sf33rd.pdb -ldbghelp
endif

# Files

MAIN_TARGET := $(BUILD_DIR)/$(MAIN)

GAME_C_FILES := $(shell find $(SRC_DIR)/sf33rd -name '*.c' 2>/dev/null)
CRI_C_FILES := $(shell find $(SRC_DIR)/cri -name '*.c' 2>/dev/null)
BIN2OBJ_C_FILES := $(shell find $(SRC_DIR)/bin2obj -name '*.c' 2>/dev/null)
PORT_C_FILES := $(shell find $(SRC_DIR)/port -name '*.c' 2>/dev/null)
ZLIB_C_FILES := $(shell find zlib -name '*.c' 2>/dev/null)

GAME_O_FILES := $(patsubst %.c,%.c.o,$(GAME_C_FILES))
GAME_O_FILES := $(addprefix $(BUILD_DIR)/,$(GAME_O_FILES))
CRI_O_FILES := $(patsubst %.c,%.c.o,$(CRI_C_FILES))
CRI_O_FILES := $(addprefix $(BUILD_DIR)/,$(CRI_O_FILES))
BIN2OBJ_O_FILES := $(patsubst %.c,%.c.o,$(BIN2OBJ_C_FILES))
BIN2OBJ_O_FILES := $(addprefix $(BUILD_DIR)/,$(BIN2OBJ_O_FILES))
PORT_O_FILES := $(patsubst %.c,%.c.o,$(PORT_C_FILES))
PORT_O_FILES := $(addprefix $(BUILD_DIR)/,$(PORT_O_FILES))
ZLIB_O_FILES := $(patsubst %.c,%.c.o,$(ZLIB_C_FILES))
ZLIB_O_FILES := $(addprefix $(BUILD_DIR)/,$(ZLIB_O_FILES))

ALL_O_FILES := $(GAME_O_FILES) $(CRI_O_FILES) $(BIN2OBJ_O_FILES) $(PORT_O_FILES) $(ZLIB_O_FILES)

build: $(MAIN_TARGET)

# Etc. rules

clean:
	git clean -fdx build

# Build

$(MAIN_TARGET): $(ALL_O_FILES) libco/build/liblibco.o
ifeq ($(PLATFORM),windows)
	@find build -name '*.o' > $(BUILD_DIR)/objects.txt
	@echo libco/build/liblibco.a >> $(BUILD_DIR)/objects.txt
	clang @$(BUILD_DIR)/objects.txt $(CLANG_LINKER_FLAGS) -o $@
	$(shell cp /mingw64/bin/SDL3.dll $(BUILD_DIR)/SDL3.dll)
else
	clang $(ALL_O_FILES) $(CLANG_LINKER_FLAGS) -o $@
endif

$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	clang -g -c $< -o $@ $(CLANG_FLAGS)

libco/build/liblibco.o:
	@mkdir -p $(dir $@)
	cd libco && \
		mkdir -p build && \
		cd build && \
		cmake .. && \
		cmake --build .
