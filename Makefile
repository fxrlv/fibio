TARGET := fibio

SRCS = $(wildcard src/*.c) $(wildcard coro/*.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)


CC ?= gcc

CFLAGS = -std=gnu11 -ggdb3 -Wall -Wextra -Werror -I. -Iinclude
LDFLAGS = -lev


.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -MP -MMD -c $< -o $@

-include $(DEPS)

.PHONY: clean
clean:
	@rm -rf $(TARGET) $(SRCS:.c=.d) $(SRCS:.c=.o)

.PHONY: fmt
fmt:
	@clang-format -i src/*.c include/*.h
