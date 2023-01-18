CC ?= gcc
CFLAGS ?= -Ofast -Wextra -Wall
LDLIBS ?= -ltps -lSDL2 -lSDL2_ttf -lm

EXEC = main

INCLUDE_PATH ?= ./include

SRC_PATH = ./src
OBJ_PATH = ./obj
BIN_PATH = ./bin
TESTS_PATH = ./tests

SOURCES  := $(wildcard $(SRC_PATH)/*.c)
INCLUDES := $(wildcard $(INCLUDE_PATH)/*.h)
OBJECTS  := $(SOURCES:$(SRC_PATH)/%.c=$(OBJ_PATH)/%.o)

# Colors for echo
GREEN='\033[32m'
L_BLUE='\033[35m'
NC='\033[0m'

all: $(BIN_PATH)/$(EXEC)

$(BIN_PATH)/$(EXEC): $(OBJECTS)
	mkdir -p $(BIN_PATH)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	mkdir -p $(OBJ_PATH)
	$(CC) -o $@ -c $< $(CFLAGS) -I$(INCLUDE_PATH)

doc:
	doxygen Doxyfile
	@echo -e ${GREEN}Documentation generated in ./docs/html/index.html${NC}

test:
	make -C $(TESTS_PATH) clean
	make -C $(TESTS_PATH) all
	@echo -e ${L_BLUE}Going into $(TESTS_PATH) for running tests${NC}
	cd $(TESTS_PATH) && ./tests
	@echo -e ${L_BLUE}Going back to root directory${NC}
	@cd ..
cov:
	make -C $(TESTS_PATH) cov
	@echo -e ${GREEN}Coverage report generated in $(TESTS_PATH)/rapport/index.html${NC}

.PHONY: clean
clean:
	rm -fr $(OBJ_PATH)
	rm -fr $(BIN_PATH)