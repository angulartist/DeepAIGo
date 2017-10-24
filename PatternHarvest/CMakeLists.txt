include_directories(${DEEPAIGO_SOURCE_DIR}/DeepAIGo)

add_executable(PatternHarvest main.cc)
target_link_libraries(PatternHarvest DeepAIGo ${Boost_LIBRARIES} mxnet)