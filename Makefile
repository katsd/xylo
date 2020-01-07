PROGNAME := xylo
CC := g++
CFLAGS := -std=c++17 -Wall -O3

PROGEXT := xy

SRCDIR := src
VMSRCDIR := $(SRCDIR)/vm
OUTDIR := build
TESTDIR := tests
TMPDIR := tmp

TARGET := $(OUTDIR)/$(PROGNAME)
VMTESTTARGET := $(OUTDIR)/vmtest

VMSRCS := $(wildcard $(VMSRCDIR)/*.cpp)
SRCS := $(wildcard $(SRCDIR)/*.cpp) $(VMSRCS)
VMTESTSRC := $(TESTDIR)/vm_test.cpp

VMOBJS := $(addprefix $(OUTDIR)/,$(patsubst %.cpp,%.o,$(VMSRCS)))
OBJS := $(addprefix $(OUTDIR)/,$(patsubst %.cpp,%.o,$(SRCS))) $(VMOBJS)
VMTESTOBJ := $(addprefix $(OUTDIR)/,$(patsubst %.cpp,%.o,$(VMTESTSRC)))

DEPENDS := $(addprefix $(TMPDIR)/,$(patsubst %.cpp,%.d,$(SRCS)))

TESTSRC := $(TESTDIR)/test.xy

all: $(TARGET)

test: all
	@./$(OUTDIR)/$(PROGNAME)  $(TESTDIR)/test.$(PROGEXT)

vmtest: $(VMTESTTARGET) all
	@./$(VMTESTTARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OUTDIR)/%.o: %.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c $<

$(VMTESTTARGET): $(VMTESTOBJ) $(VMOBJS)
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(OUTDIR)
	rm -rf $(TMPDIR)

.PHONY: depend
depend: $(DEPENDS)

$(TMPDIR)/%.d: %.cpp
	@mkdir -p $(dir $@)
	@$(CC) -MM $< | sed -e '1 s:^:$(dir $@):g' | sed -e '1 s/^$(TMPDIR)/$(OUTDIR)/g' > $@

-include $(DEPENDS)