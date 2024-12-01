.PHONY: all clean
.DEFAULT_GOAL = all

DIR_BUILD = build

CC = gcc
CFLAGS =\
	-c -std=c23\
	-Wall -Wextra -Wpedantic\
	-MMD
LDFLAGS =\
	-lm
ifdef DEBUG
	DIR_OBJ = $(DIR_BUILD)/debug
	CFLAGS +=\
		-fsanitize=undefined -fsanitize-undefined-trap-on-error\
		-g\
		-O0
	LDFLAGS +=\
		-fsanitize=undefined -fsanitize-undefined-trap-on-error\
		-Wl,-Map -Wl,$(DIR_OBJ)/$(TARGET_NAME).map
else
	DIR_OBJ = $(DIR_BUILD)/release
	CFLAGS +=\
		-DNDEBUG\
		-O1
	LDFLAGS +=
endif

OBJECTS = $(addprefix $(DIR_OBJ)/, $(SOURCES:.c=.o))
DEPENDS = $(addprefix $(DIR_OBJ)/, $(SOURCES:.c=.d))
INCLUDES = $(addprefix -I, $(INCLUDE_PATHS))

-include $(DEPENDS)

all: $(DIR_OBJ) $(DIR_OBJ)/$(TARGET_NAME)

$(DIR_OBJ)/$(TARGET_NAME): $(OBJECTS) | $(DIR_OBJ)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(DIR_OBJ)/%.o: %.c | $(DIR_OBJ)
	$(CC) $(INCLUDES) $(DEFINES) $(CFLAGS) -c $< -o $@

$(DIR_OBJ):
	@mkdir -p $(DIR_OBJ)

clean:
	@rm -rfd $(DIR_BUILD)
