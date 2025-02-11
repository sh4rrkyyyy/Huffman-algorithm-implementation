CXX = g++
CXXFLAGS = -O2 -Wall -Werror -std=c++11 -Iinclude
LDFLAGS =

TEST_EXE = hw-02_huffman_test
EXE = hw-02_huffman
SRCDIR = src
OBJDIR = obj
TESTDIR = test

OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.cpp))

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXE) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<

include $(wildcard $(OBJDIR)/*.d)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(EXE) $(TEST_EXE)

test: $(OBJDIR) $(TEST_EXE)

$(TEST_EXE): $(OBJDIR)/huffman.o $(OBJDIR)/huffman_test.o
	$(CXX) $(OBJDIR)/huffman.o $(OBJDIR)/huffman_test.o -o $(TEST_EXE) $(LDFLAGS)

$(OBJDIR)/huffman.o: $(SRCDIR)/huffman.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -MMD -o $(OBJDIR)/huffman.o $(SRCDIR)/huffman.cpp

$(OBJDIR)/huffman_test.o: $(TESTDIR)/huffman_test.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -MMD -o $(OBJDIR)/huffman_test.o $(TESTDIR)/huffman_test.cpp

.PHONY: clean all test
