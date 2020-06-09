add_executable(Admin_exe Admin.cpp)
add_executable(Viewer_exe Viewer.cpp)

target_link_libraries(Admin_exe iVote Star FifoManager)
target_link_libraries(Viewer_exe FifoManager)

install(TARGETS Admin_exe RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
install(TARGETS Viewer_exe RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
