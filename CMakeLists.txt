## Set CMake Minimum Version
cmake_minimum_required(VERSION 3.27.9)

## C++ Version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

## Project Name and Software Version Number
project(AutonomyThread_Benchmark VERSION 24.02.00 LANGUAGES CXX CUDA)
set(EXE_NAME "${PROJECT_NAME}")

# Configure BSThreadPool
add_compile_definitions(BS_THREAD_POOL_ENABLE_PAUSE=1)
add_compile_definitions(BS_THREAD_POOL_ENABLE_PRIORITY=1)
add_compile_definitions(BS_THREAD_POOL_ENABLE_WAIT_DEADLOCK_CHECK=1)

## Allows use of "FindCUDA" function
cmake_policy(SET CMP0146 OLD)

## Determine if all variables should be listed. Nice for finding available vars when using find_package.
option(LIST_ALL_VARS "Print all CMAKE variables." OFF)

## Check if all variables should be listed.
if (LIST_ALL_VARS)
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endif()

## Find CUDA
find_package(CUDAToolkit 12.2 REQUIRED)

## Find Threads
find_package(Threads REQUIRED)

## Find Eigen3.
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
## Find OpenCV.
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

## Search Project Directories for CPP Files
file(GLOB_RECURSE SRC			  	    CONFIGURE_DEPENDS  "src/*.cpp")

## Create Executable File
add_executable(${EXE_NAME} ${SRC})

## Set Compile Options for program.
if(MSVC) # True when compiler is Microsoft Visual C++/simulation of Visual C++ CL.
	target_compile_options(${EXE_NAME} PRIVATE /W4 /WX)
else()
	target_compile_options(${EXE_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

## Set program Libraries
set(AUTONOMYTHREAD_BENCHMARK_LIBRARIES  Threads::Threads Eigen3::Eigen ${OpenCV_LIBS} ${CUDAToolkit_LIBRARIES})

## Link Libraries to Executable
target_link_libraries(${EXE_NAME} PRIVATE ${AUTONOMYTHREAD_BENCHMARK_LIBRARIES})