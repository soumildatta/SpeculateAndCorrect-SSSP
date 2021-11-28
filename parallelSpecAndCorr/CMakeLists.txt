cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++17")

project(ParallelSpecAndCorr)

add_executable(
	ParallelSpecAndCorr
	main.cpp
	ForEach.h
	tData.h
	tEdgeList.h
	tGraph.h
	tPool.h
	tTimer.h
	UtilityFunctions.h
	validation.cpp
	validation.h
)