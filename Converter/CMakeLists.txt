include_directories(${DEEPAIGO_SOURCE_DIR}/DeepAIGo)

add_executable(Converter Converter.cc main.cc)
target_link_libraries(Converter DeepAIGo ${Boost_LIBRARIES} ${HDF5_LIBRARIES})