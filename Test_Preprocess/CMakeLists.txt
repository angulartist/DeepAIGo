include_directories(${DEEPAIGO_SOURCE_DIR}/DeepAIGo)

add_executable(Test_Preprocess main.cc)
target_link_libraries(Test_Preprocess DeepAIGo)