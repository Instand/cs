set(TEST_NAME test_program)

file(GLOB SRCS *.cpp)
add_executable(${TEST_NAME} ${SRCS})

set_property(TARGET ${TEST_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${TEST_NAME} PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)
