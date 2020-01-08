PROG_NAME := xylo
CC := g++
CFLAGS := -std=c++17 -Wall -O3

PROG_EXT := xy

SRC_DIR := src
VM_SRC_DIR := $(SRC_DIR)/vm
OUT_DIR := build
TEST_DIR := tests
TMP_DIR := tmp

TARGET := $(OUT_DIR)/$(PROG_NAME)
TEST_TARGET := $(OUT_DIR)/xytest
VM_TEST_TARGET := $(OUT_DIR)/vmtest

MAIN_SRC := $(SRC_DIR)/main.cpp
TEST_SRC := $(TEST_DIR)/test.cpp
VM_TEST_SRC := $(TEST_DIR)/vm_test.cpp
VM_SRCS := $(wildcard $(VM_SRC_DIR)/*.cpp)
SRCS := $(filter-out $(MAIN_SRC),$(wildcard $(SRC_DIR)/*.cpp)) $(VM_SRCS)

VM_OBJS := $(addprefix $(OUT_DIR)/,$(patsubst %.cpp,%.o,$(VM_SRCS)))
OBJS := $(addprefix $(OUT_DIR)/,$(patsubst %.cpp,%.o,$(SRCS))) $(VM_OBJS)
MAIN_OBJ := $(addprefix $(OUT_DIR)/,$(patsubst %.cpp,%.o,$(MAIN_SRC)))
TEST_OBJ := $(addprefix $(OUT_DIR)/,$(patsubst %.cpp,%.o,$(TEST_SRC)))
VM_TEST_OBJ := $(addprefix $(OUT_DIR)/,$(patsubst %.cpp,%.o,$(VM_TEST_SRC)))

DEPENDS := $(addprefix $(TMP_DIR)/,$(patsubst %.cpp,%.d,$(SRCS) $(MAIN_SRC)))

TEST_SRC := $(TEST_DIR)/test.xy

all: $(TARGET)

test: $(TEST_TARGET) $(OBJS)
	@./$(TEST_TARGET)

vmtest: $(VM_TEST_TARGET) $(OBJS)
	@./$(VM_TEST_TARGET)

$(TARGET): $(OBJS) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OUT_DIR)/%.o: %.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c $<

$(TEST_TARGET): $(TEST_OBJ) $(OBJS)
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ $^

$(VM_TEST_TARGET): $(VM_TEST_OBJ) $(VM_OBJS)
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(OUT_DIR)
	rm -rf $(TMP_DIR)

.PHONY: depend
depend: $(DEPENDS)

$(TMP_DIR)/%.d: %.cpp
	@mkdir -p $(dir $@)
	@$(CC) -MM $< | sed -e '1 s:^:$(dir $@):g' | sed -e '1 s/^$(TMP_DIR)/$(OUT_DIR)/g' > $@

-include $(DEPENDS)