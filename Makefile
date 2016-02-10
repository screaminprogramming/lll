CXX = g++
CPPFLAGS = -I include
CXXFLAGS = -Wall -Wextra -g -std=c++14
LDFLAGS = -lc++

# we want to make all cpp files in the unittest directory
#SRCS    = $(wildcard unittest/*.cpp)
SRCS    = unittest/logger.cpp unittest/astack.cpp unittest/object_pool.cpp
#OBJS    = ${SRCS:.cpp=.o}
OBJS    = unittest/logger.o unittest/astack.o unittest/object_pool.o

test/unittest: ${OBJS} test/unittest.o

test/unittest.o: test/unittest.cpp include/lll/lll_unittest.h

# declare that each unit test is dependent on its header file
unittest/%.o: unittest/%.cpp include/lll/%.h include/lll/lll_unittest.h
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<


