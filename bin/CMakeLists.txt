add_executable(metadata_parser main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ID3_Manipulator)
target_link_libraries(${PROJECT_NAME} PRIVATE TagHeader)
target_link_libraries(${PROJECT_NAME} PRIVATE Utility)
