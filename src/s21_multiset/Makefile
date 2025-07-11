CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -fprofile-arcs -ftest-coverage
LDFLAGS = -lgtest -lgtest_main -pthread -fprofile-arcs -ftest-coverage

SRC = s21_multiset_test.cpp
OUT = set_test

all: test coverage_html

test: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)
	./$(OUT)

coverage_html: test
	gcovr -r . --html --html-details -o coverage_report.html
	xdg-open coverage_report.html

clean:
	rm -f *.o *.gcno *.gcda $(OUT) coverage_report.html coverage*.*

