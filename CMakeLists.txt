cmake_minimum_required(VERSION 3.0.0)
project(neide VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# sudo apt install libwxbase3.0-dev libwxgtk3.0-gtk3-dev
find_package(wxWidgets REQUIRED core base)
include(${wxWidgets_USE_FILE})

add_executable(${PROJECT_NAME} main.cpp cpp_tpl.h utils.cpp)
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
