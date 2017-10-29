include_directories(${DEEPAIGO_SOURCE_DIR}/DeepAIGo)

add_executable(Test_MCTS main.cc)
target_link_libraries(Test_MCTS DeepAIGo mxnet)