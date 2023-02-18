add_executable(metadata_parser main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ID3_Manipulator)
target_link_libraries(${PROJECT_NAME} PRIVATE Frame)
target_link_libraries(${PROJECT_NAME} PRIVATE FrameHeader)
target_link_libraries(${PROJECT_NAME} PRIVATE FrameList)
target_link_libraries(${PROJECT_NAME} PRIVATE Tag)
target_link_libraries(${PROJECT_NAME} PRIVATE TagHeader)
target_link_libraries(${PROJECT_NAME} PRIVATE Utility)
