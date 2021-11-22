cmake_minimum_required(VERSION 3.14)

project(spaceship LANGUAGES CXX)

find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${CMAKE_PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/load_shaders.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_17)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${GLUT_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${OpenGL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    GLUT::GLUT
    GLEW::GLEW
    OpenGL::GL
    OpenGL::GLU)

macro(copy_file FILE_NAME EXECUTABLE_NAME)
  add_custom_command(
    TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/src/${FILE_NAME}
            ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME})
endmacro()

copy_file(spaceship_shader.vert ${CMAKE_PROJECT_NAME})
copy_file(spaceship_shader.frag ${CMAKE_PROJECT_NAME})

copy_file(background_shader.vert ${CMAKE_PROJECT_NAME})
copy_file(background_shader.frag ${CMAKE_PROJECT_NAME})

copy_file(flames_shader.vert ${CMAKE_PROJECT_NAME})
copy_file(flames_shader.frag ${CMAKE_PROJECT_NAME})

copy_file(RocketFlames.png ${CMAKE_PROJECT_NAME})
