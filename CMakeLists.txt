cmake_minimum_required(VERSION 3.8)
project(OOP_lab5)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
	src/main.cpp
	src/PrimeFactors.h
	src/PrimeFactors.cpp
	src/ThreadPool.h
	src/ThreadPool.cpp
	)

add_executable(OOP_lab5 ${SOURCE_FILES})