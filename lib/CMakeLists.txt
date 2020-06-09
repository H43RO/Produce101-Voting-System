add_library(iVote STATIC iVote.cpp)

add_library(Star STATIC Star.cpp)

add_library(FifoManager STATIC FifoManager.cpp)

install(TARGETS iVote ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

install(TARGETS Star ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

install(TARGETS FifoManager ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
