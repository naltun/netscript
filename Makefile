BIN    = netscript
BUILD  = build
CC 	  ?= cc
CFLAGS = -Wall -Wextra
OBJS   = $(SRCS:%.c=$(BUILD)/%.o)
SRCS   = main.c parser.c tokenizer.c
TARGET = $(BUILD)/$(BIN)

$(BUILD):
	mkdir -p $(BUILD)

all: $(TARGET)

$(TARGET): $(BUILD) $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

$(BUILD)/%.o: %.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

#
# Build each object file separately prior to linking, ergo create `tcc' target
# in place of `make CC=tcc ...'. This works more nicely with tcc's dependency
# resolution.
#
tcc: $(BUILD)
	tcc -c tokenizer.c -o $(BUILD)/tokenizer.o
	tcc -c parser.c -o $(BUILD)/parser.o
	tcc -c main.c -o $(BUILD)/main.o
	tcc $(OBJS) -o $(TARGET)

clean:
	rm -rf $(BUILD)

.PHONY: all clean tcc
