project(project)
cmake_minimum_required(VERSION 3.10)

include_directories(inc)
add_subdirectory(src)
add_subdirectory(lib)

add_dependencies(Admin_exe iVote Star FifoManager)
add_dependencies(Viewer_exe FifoManager)
