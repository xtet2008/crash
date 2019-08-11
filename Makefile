# 编译好的可执行程序的名称
TARGET = crash

# 编译器，这里使用gcc
CC = gcc
# 编译器标志(Flag)
CFLAGS = -std=c99 -Wall -I. -g
#CFLAGS   = -std=c99 -Wall -I.

# 连接器，继续使用 gcc
LINKER = gcc
# 连接器标志
LFLAGS   = -Wall -I. -lm

# 源代码目录
SRCDIR = src
# 目标文件目录
OBJDIR = obj
# 二进制可执行文件目录
BINDIR = bin

# 所有源代码文件
SOURCES := $(shell find ./src -name "*.c")
# 所有头文件
INCLUDES := $(shell find ./src -name "*.h")
# 所有目标文件
OBJECTS :=$(SOURCES:$(SRCDIR)/%.c=(OBJDIR)/%.o)

# 连接规则
$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

# 编译规则
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

# 清理编译文件
.PHONY: clean
clean:
	@rm -rf $(BINDIR) $(OBJDIR)
	@echo "Cleanup complete!"
