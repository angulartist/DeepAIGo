include_directories(${DEEPAIGO_SOURCE_DIR}/DeepAIGo)

add_executable(SLPolicy HDF5Provider.cc PolicyTrainer.cc main.cc)
target_link_libraries(SLPolicy DeepAIGo ${Boost_LIBRARIES} mxnet ${HDF5_LIBRARIES})