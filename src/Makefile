CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -fprofile-arcs -ftest-coverage -fprofile-update=atomic
LDFLAGS = -lgtest -lpthread

SRC = \
  s21_vector/test_vector.cpp \
  s21_array/test_array.cpp \
  s21_map/test_map.cpp \
  s21_queue/test_queue.cpp \
  s21_list/s21_list_test.cpp \
  s21_multiset/s21_multiset_test.cpp \
  s21_set/s21_set_test.cpp \
  s21_stack/s21_stack_test.cpp \
  tests.cpp

SRC_FILES := $(shell find . -name "*.cpp" -o -name "*.h")
OUT = test

UNAME = $(shell uname)

ifeq ($(UNAME), Darwin)
    OPEN = open
else
    OPEN = xdg-open
endif

all: clean test coverage_html

test:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)
	./$(OUT)

coverage_html: test
	gcovr -r . \
		--exclude s21_map/rb_tree.h \
		--html --html-details -o coverage_report.html
	$(OPEN) coverage_report.html

clean:
	rm -f *.o *.gcno *.gcda *.gcov $(OUT) coverage_report.html coverage*.*

clang_format: clean
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i $(SRC_FILES)
	rm -f .clang-format

check_format:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n $(SRC_FILES)
	rm -f .clang-format
