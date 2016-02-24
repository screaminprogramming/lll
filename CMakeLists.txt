cmake_minimum_required(VERSION 3.3)
project(lll)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
include_directories("${CMAKE_SOURCE_DIR}/include")
set(SOURCE_FILES
    include/lll/astack.h
    include/lll/cacheline.h
    include/lll/constants.h
    include/lll/lll_unittest.h
    include/lll/logger.h
    include/lll/object_pool.h
    test/unittest.cpp
    unittest/astack.cpp
    unittest/logger.cpp
    unittest/object_pool.cpp
    LICENCE
    README)

add_executable(unittest ${SOURCE_FILES})