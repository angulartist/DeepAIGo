include_directories(${DEEPAIGO_SOURCE_DIR}/DeepAIGo)

add_executable(Test_TTScore main.cc)
target_link_libraries(Test_TTScore DeepAIGo)