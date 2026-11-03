cmake_minimum_required(VERSION 3.16)
project(code_generator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GENERATOR_SOURCES
    unit.h
    classunit.h
    methodunit.h
    printoperatorunit.h
    factory.h
    cppfactory.h
    cppfactory.cpp
    csharpfactory.h
    csharpfactory.cpp
    javafactory.h
    javafactory.cpp
)

add_executable(${PROJECT_NAME} ${GENERATOR_SOURCES} main.cpp)

add_executable(validation_tests ${GENERATOR_SOURCES} tests/validation_tests.cpp)

enable_testing()
add_test(NAME validation_tests COMMAND validation_tests)
