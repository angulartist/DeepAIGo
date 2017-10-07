cmake_minimum_required(VERSION 3.2)
project(DEEPAIGO)

set(CMAKE_CXX_STANDARD 14)

find_package(HDF5 REQUIRED)
find_package(Boost 1.64 COMPONENTS program_options filesystem system REQUIRED)

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
add_subdirectory(SLPolicy)

add_subdirectory(Test_Board)
add_subdirectory(Test_Preprocess)
add_subdirectory(Test_SGFParser)