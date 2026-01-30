CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -fprofile-arcs -ftest-coverage -lgtest -fprofile-update=atomic

SRC = test_queue.cpp
OUT = test

all: test coverage_html

test:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)
	./$(OUT)

coverage_html: test
	gcovr -r . --html --html-details -o coverage_report.html
	open coverage_report.html

clean:
	rm -f *.o *.gcno *.gcda $(OUT) coverage_report.html coverage*.*
