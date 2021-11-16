#ifndef LOAD_SHADERS_HPP
#define LOAD_SHADERS_HPP

#include <GL/glew.h>

auto load_shaders(const char* vertex_file_path, const char* fragment_file_path) -> GLuint;

#endif // !LOAD_SHADERS_HPP