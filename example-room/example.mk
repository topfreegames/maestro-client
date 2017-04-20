CC=g++
LDLIBS=-L../cpplib/lib -static -lmaestro -lrestclient-cpp -lpthread -lcurl
EXECUTABLE ?= bin/example
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -std=c++11

$(EXECUTABLE): $(OBJS)
	mkdir -p bin && $(CC) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

all: $(EXECUTABLE)

.PHONY: clean
clean:
	$(RM) $(EXECUTABLE) $(OBJS) $(DEPS)

-include $(DEPS)
