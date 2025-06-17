CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -fprofile-arcs -ftest-coverage -lgtest -fprofile-update=atomic

SRC = test_map.cpp
OUT = test

all: test coverage_html

test:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)
	./$(OUT)

coverage_html: test
	gcovr -r . --html --html-details -o coverage_report.html
	open coverage_report.html
	

clean:
	rm -f *.o *.gcno *.gcda *.gcov *.out $(OUT) coverage_report.html coverage*.*

clang_format: clean
	cp ../../materials/linters/.clang-format .clang-format
	clang-format -i *.cpp *.h
	rm -f .clang-format

check_format:
	cp ../../materials/linters/.clang-format .clang-format
	clang-format -n *.cpp *.h
	rm -f .clang-format
