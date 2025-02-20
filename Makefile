LD = clang

PATH_BIN = bin
OUT = $(PATH_BIN)/out
PATH_SRC = src
PATH_SHADERS = src/shader
PATH_LIB = lib

SRC = $(shell find $(PATH_SRC) -name "*.c")
OBJ = $(SRC:%.c=$(PATH_BIN)/%.o)
DEPS = $(SRC:%.c=$(PATH_BIN)/%.d)


# SHDSRC = $(shell find $(SHADERS) -name "*.glsl")
# SHDOUT = $(SDHSRC:%.glsl=%.glsl.h)

CFLAGS = -Wall
CFLAGS += -Wpedantic
CFLAGS += -Wextra
CFLAGS += -Wno-gnu
CFLAGS += -std=c99 
CFLAGS += $(shell pkg-config -cflags sdl3)
# CFLAGS += $(shell pkg-config -libs- sdl3)

LDFLAGS = $(shell pkg-config -libs sdl3)
# LDFLAGS += -lm -lstdc++

dirs:
	$(shell mkdir -p $(PATH_BIN))
	rsync -a --include "*/" --exclude "*" "$(PATH_SRC)" "$(PATH_BIN)"
	rsync -a --include "*/" --exclude "*" "$(PATH_LIB)" "$(PATH_BIN)"

$(OBJ): $(PATH_BIN)/%.o: %.c dirs
	$(CC) -o $@ -c -MMD $(CFLAGS) $(LDFLAGS) $<

package: $(OBJ)
	$(LD) -o $(OUT) $(filter %.o,$^) $(LDFLAGS)

run: package
	./$(OUT)

clean:
	$(shell rm -r $(PATH_BIN))
