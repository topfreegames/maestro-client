CC=g++
EXECUTABLE ?= bin/example
SRC_DIRS ?= ./src
LIB=lib/libmaestro.a

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -std=c++11

$(LIB): $(OBJS)
	@mkdir -p lib && ar rcs $(LIB) $(OBJS) $(DEPS)

all: $(LIB)

.PHONY: clean
clean:
	$(RM) $(LIB) $(OBJS) $(DEPS)

-include $(DEPS)
