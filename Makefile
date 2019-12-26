PROGNAME := xylo
CC := g++
CFLAGS := -std=c++17 -Wall -O3

SRCDIR := src
VMSRCDIR := $(SRCDIR)/vm
OUTDIR := build
TESTDIR := tests

TARGET := $(OUTDIR)/$(PROGNAME)
VMTESTTARGET := $(OUTDIR)/vmtest

VMSRCS := $(wildcard $(VMSRCDIR)/*.cpp)
SRCS := $(wildcard $(SRCDIR)/*.cpp) $(VMSRCS)
VMTESTSRC := $(TESTDIR)/vm_test.cpp

VMOBJS := $(addprefix $(OUTDIR)/,$(patsubst %.cpp,%.o,$(VMSRCS)))
OBJS := $(addprefix $(OUTDIR)/,$(patsubst %.cpp,%.o,$(SRCS))) $(VMOBJS)
VMTESTOBJ := $(addprefix $(OUTDIR)/,$(patsubst %.cpp,%.o,$(VMTESTSRC)))

all: $(TARGET)

vmtest: $(VMTESTTARGET)
	@./$(VMTESTTARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OUTDIR)/%.o: %.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c $<

$(VMTESTTARGET): $(VMTESTOBJ) $(VMOBJS)
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OUTDIR)
	rm $(PROGNAME)
