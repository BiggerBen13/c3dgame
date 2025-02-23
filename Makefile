PATH_BIN = bin
TARGET-NAME = $(PATH_BIN)/out

PATH_SRC = src
PATH_SHADER = $(PATH_SRC)/shader
PATH_LIB = lib

PATH_SOKOL = $(PATH_LIB)/sokol
PATH_SOKOL_TOOLS = $(PATH_LIB)/sokol-tools-bin

SRC = $(shell find $(PATH_SRC) -name "*.c")
OBJ = $(SRC:%.c=$(PATH_BIN)/%.o)
DEPS = $(SRC:%.c=$(PATH_BIN)/%.d)

SRC_SHD = $(shell find $(PATH_SHADER) -name "*.glsl")
OUT_SHD = $(SRC_SHD:%.glsl=%.glsl.h)

CFLAGS = -Wall
CFLAGS += -Wpedantic
CFLAGS += -Wextra
CFLAGS += -Wno-gnu
CFLAGS += -std=c99 

ifdef RELEASE
	CFLAGS += -O2
else
	CFLAGS += -g
endif


CFLAGS += $(shell pkg-config --cflags-only-other sdl3)

INCFLAGS = -I$(PATH_SOKOL)
INCFLAGS += $(shell pkg-config --cflags-only-I sdl3)

LDFLAGS = $(shell pkg-config -libs sdl3)
LDFLAGS += -lm -lstdc++

UNAME = $(shell uname -s)

ifndef $(HOST_PLATFORM)
	ifeq ($(UNAME),Darwin)
	HOST_PLATFORM = macos
	HOST_ARCH = $(shell uname -m)
else ifeq ($(UNAME),Linux)
	HOST_PLATFORM = linux
	HOST_ARCH = x86_64 # TODO: assert
else
	HOST_PLATFORM = windows
	HOST_ARCH = x86_64 # TODO: assert
endif
endif

ifeq ($(HOST_PLATFORM),macos)
	ifeq ($(HOST_ARCH),arm64)
	SHDC = $(PATH_SOKOL_TOOLS)/bin/osx_arm64/sokol-shdc
else
	SHDC = $(PATH_SOKOL_TOOLS)/bin/osx/sokol-shdc
endif
else ifeq ($(HOST_PLATFORM),linux)
	SHDC = $(PATH_SOKOL_TOOLS)/bin/linux/sokol-shdc
else
	SHDC = $(PATH_SOKOL_TOOLS)/bin/win32/sokol-shdc.exe
endif

ifeq ($(HOST_PLATFORM),macos)
ifdef metal
	LDFLAGS += -framework Metal
else
	LDFLAGS += -framework OpenGl
endif

else ifeq ($(HOST_PLATFORM),linux)
	LDFLAGS += -lGL
endif

LD = clang
CC = clang

-include $(DEPS)

dirs:
	$(shell mkdir -p $(PATH_BIN))
	rsync -a --include "*/" --exclude "*" "$(PATH_SRC)" "$(PATH_BIN)"
	rsync -a --include "*/" --exclude "*" "$(PATH_LIB)" "$(PATH_BIN)"

$(OBJ): $(PATH_BIN)/%.o: %.c dirs
	$(CC) -o $@ -c -MD $(CFLAGS) $(INCFLAGS) $<

objs: $(OBJ)

$(OUT_SHD): %.glsl.h: %.glsl
	$(SHDC) --input $^ --output $@ --slang glsl300es:glsl410 --format=sokol_impl


shader: $(OUT_SHD)

package: $(OBJ)
	$(LD) -o $(TARGET-NAME) $(filter %.o,$^) $(LDFLAGS)

run: package
	./$(TARGET-NAME)

clean:
	$(shell rm -r $(PATH_BIN))
