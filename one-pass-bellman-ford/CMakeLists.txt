cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++17")

project(specAndCorr)

add_executable(
	specAndCorr
	main.cpp
	optimizedBellmanFord.cpp
	optimizedBellmanFord.h
	UtilityFunctions.h
	ForEach.h
	tEdgeList.h
	tGraph.h
	tTimer.h
	validation.cpp
	validation.h
	tSSSPPerformanceCounters.h
)