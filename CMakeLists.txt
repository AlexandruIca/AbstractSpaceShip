cmake_minimum_required(VERSION 3.14)

project(spaceship LANGUAGES CXX)

find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${GLUT_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${OpenGL_INCLUDE_DIRS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    ${GLUT_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OpenGL_LIBRARIES})
