cmake_minimum_required(VERSION 3.16)
project(code_generator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    unit.h
    classunit.h
    methodunit.h
    printoperatorunit.h
    factory.h
    cppfactory.h
    cppfactory.cpp
    csharpfactory.h
    csharpfactory.cpp
    main.cpp
)
