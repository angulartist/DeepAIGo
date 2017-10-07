include_directories(${DEEPAIGO_SOURCE_DIR}/DeepAIGo)

add_executable(AIGoGtp main.cc)
target_link_libraries(AIGoGtp DeepAIGo ${Boost_LIBRARIES} mxnet)