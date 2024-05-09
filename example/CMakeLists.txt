# link_directories(${PROJECT_SOURCE_DIR}/bin)
# set(CMAKE_CXX_STANDARD 17)
add_executable(test main.cpp)
target_link_libraries(test threadpool)