cmake_minimum_required(VERSION 3.2)
project(DEEPAIGO)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(HDF5 REQUIRED)
find_package(Boost 1.64 COMPONENTS program_options filesystem system REQUIRED)
find_package(Threads)

include_directories(${Boost_INCLUDE_DIR})
include_directories($ENV{MXNET_HOME}/include)
include_directories($ENV{MXNET_HOME}/nnvm/include)
include_directories($ENV{MXNET_HOME}/dmlc-core/include)
include_directories($ENV{MXNET_HOME}/cpp-package/include)
include_directories(${HDF5_INCLUDE_DIRS})
link_directories($ENV{MXNET_HOME}/lib)

add_subdirectory(DeepAIGo)
add_subdirectory(AIGo_Gtp)
add_subdirectory(Converter)

add_subdirectory(Test_Board)
add_subdirectory(Test_Preprocess)
add_subdirectory(Test_SGFParser)
add_subdirectory(Test_Symmetrics)
add_subdirectory(Test_TTScore)
add_subdirectory(Test_MCTS)