CC=mpicc -Wall -Wextra -std=gnu99 -lm
CFLAGS=-I.

BUILD_DIR := build
OBJ_DIR := build/src
OBJ_TEST_DIR := $(BUILD_DIR)/test/src


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

HEADERS = include/*.h

TESTS := pa1_test
all :leibniz_main monte-carlo_main matrix-vector_main matrix-matrix_main matrix-matrix-cannon $(GTEST_LIBS) ${TESTS}


$(OBJ_DIR)/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) -c $< -o $@

$(OBJ_DIR)/%.o: src/leibniz.c include/leibniz_main.h
	mkdir -p $(@D)
	$(CC) -c $< -o $@

leibniz_main: $(OBJ_DIR)/leibniz.o $(OBJ_DIR)/leibniz_main.o
	mkdir -p build/bin
	$(CC) $^ -o build/bin/$@

$(OBJ_DIR)/%.o: src/monte-carlo.c include/monte-carlo.h
	mkdir -p $(@D)
	$(CC) -c $< -o $@

monte-carlo_main: $(OBJ_DIR)/monte-carlo.o $(OBJ_DIR)/monte-carlo_main.o
	mkdir -p build/bin
	$(CC) $^ -o build/bin/$@

lib/%.o: lib/mmio.c lib/mmio.h
	mkdir -p $(@D)
	$(CC) -c $< -o $@

$(OBJ_DIR)/%.o: src/matrix-vector.c include/matrix-vector.h
	mkdir -p $(@D)
	$(CC) -c $< -o $@

matrix-vector_main: lib/mmio.o $(OBJ_DIR)/matrix-vector.o $(OBJ_DIR)/matrix-vector_main.o
	mkdir -p build/bin
	$(CC) $^ -o build/bin/$@

$(OBJ_DIR)/%.o: src/matrix-matrix.c include/matrix-matrix.h
	mkdir -p $(@D)
	$(CC) -c $< -o $

matrix-matrix_main: lib/mmio.o $(OBJ_DIR)/matrix-matrix.o $(OBJ_DIR)/matrix-matrix_main.o
	mkdir -p build/bin
	$(CC) $^ -o build/bin/$@

$(OBJ_DIR)/%.o: src/matrix-matrix-cannon.c
	mkdir -p $(@D)
	$(CC) -c $< -o $

matrix-matrix-cannon: lib/mmio.o $(OBJ_DIR)/matrix-matrix.o $(OBJ_DIR)/matrix-matrix-cannon.o
	mkdir -p build/bin
	$(CC) $^ -o build/bin/$@
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

$(BUILD_DIR)/test/src/%.o: src/%.c  ${HEADERS} $(GTEST_HEADERS)
	mkdir -p $(@D)
	$(CC) -c $< -o $@

$(BUILD_DIR)/test/pa1_test.o : test/pa1_test.c ${HEADERS} $(GTEST_HEADERS) 
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c test/pa1_test.c -o $@

pa1_test : $(BUILD_DIR)/test/pa1_test.o $(GBUILD)/libgtest_main.a $(OBJ_TEST_DIR)/leibniz.o $(OBJ_TEST_DIR)/monte-carlo.o $(OBJ_TEST_DIR)/matrix-vector.o   $(OBJ_TEST_DIR)/matrix-matrix.o lib/mmio.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o build/bin/$@
	./var/mv_test.sh
	./build/bin/pa1_test