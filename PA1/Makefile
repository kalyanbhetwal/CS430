CC=gcc
CFLAGS=-I.

BUILD_DIR := build
OBJ_DIR := build/src
SRCS := hellofunc.c hellomake.c
OBJS :=  $(OBJ_DIR)/hellomake.o $(OBJ_DIR)/hellofunc.o # output object files
TESTS:= test/pa1_test

GTEST_DIR := lib/gtest/googletest
GBUILD :=  build/bin/gtest

CPPFLAGS += -isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread -std=c++11

# Google Test libraries
GTEST_LIBS = $(GBUILD)/libgtest.a  $(GBUILD)/libgtest_main.a

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h


TESTS := pa1_test
all : $(BUILD_DIR)/bin/program $(GTEST_LIBS) ${TESTS}

.PHONY: program
	program: $(BUILD_DIR)/bin/program

$(OBJ_DIR)/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) -c $< -o $@

$(BUILD_DIR)/bin/program: $(OBJS)
	mkdir -p $(@D)
	$(CC) $^ -o $@

clean:
	rm -rf $(BUILD_DIR)/*


GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
GMOCK_SRCS_ = $(GMOCK_DIR)/src/*.cc $(GMOCK_HEADERS)

# For simplicity and to avoid depending on implementation details of
# Google Mock and Google Test, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Mock and
# Google Test compile fast and for ordinary users their source rarely
# changes.
$(GBUILD)/gtest-all.o : $(GTEST_SRCS_)
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR)  $(CXXFLAGS) \
            -c $(GTEST_DIR)/src/gtest-all.cc -o $@

$(GBUILD)/gtest_main.o : $(GTEST_SRCS_)
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) \
            -c $(GTEST_DIR)/src/gtest_main.cc -o $@

$(GBUILD)/libgtest.a : $(GBUILD)/gtest-all.o
	mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $^

$(GBUILD)/libgtest_main.a : $(GBUILD)/gtest-all.o $(GBUILD)/gtest_main.o
	mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $^

pa1_test.o : test/pa1_test.c $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c test/pa1_test.c

test_mihai : pa1_test.o $(GBUILD)/libgtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@