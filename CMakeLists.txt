cmake_minimum_required(VERSION 3.14)
project(MShopMachineObservation)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(MShopMachineObservation main.cpp csv-related.cpp data.cpp defineVariables.cpp graphics.cpp loop.cpp miscFunc.cpp modesStates.cpp mouseFuncs.cpp testFunctions.cpp txt-related.cpp csvgraphics.h csvgraphicsData.h overlap.h)
target_link_libraries(MShopMachineObservation -lOpenGL32 -lglu32 -lfreeglut)