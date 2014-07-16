CXX = g++
CPPFLAGS = -I include
CXXFLAGS = -Wall -Wextra -g -std=c++0x

test/tester: test/tester.o

test/tester.o: test/tester.cpp include/logger.h
