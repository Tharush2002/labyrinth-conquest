# Compiler
CC = gcc
# Compiler flags
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lm
# Executable name
TARGET = maze_runner
# Source files
SRCS = main.c maze.c
# Object files (replace .c with .o)
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET) log_file.txt

# Run the program (line-buffered output, log saved)
run: $(TARGET)
	stdbuf -oL -eL ./$(TARGET) 2>&1 | tee log_file.txt
