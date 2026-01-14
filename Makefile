# 1. Variables
CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = lcs_app

# 2. List of source files
# Add any new .c files here as your project grows
SRCS = lcs_return_single.c file_reader.c dynamic_single_string.c

# 3. Object files (automatically generated from SRCS)
# This replaces .c with .o for all files in SRCS
OBJS = $(SRCS:.c=.o)

# 4. Default rule: Build the final executable
all: $(TARGET)

# 5. Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# 6. Rule for individual object files
# This tells make how to turn a .c file into a .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 7. Clean rule to remove generated files
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
