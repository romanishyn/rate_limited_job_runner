cmake_minimum_required(VERSION 3.15)

# project name and enable C++ support
project(hello LANGUAGES CXX)

# require C++11
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# we define the executable
add_executable(main.o "")

# and its sources
target_sources(
  main.o
  PRIVATE
    main.cpp
  )