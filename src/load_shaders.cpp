#include "load_shaders.hpp"

#include <string_view>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

enum class shader_type
{
    vertex = GL_VERTEX_SHADER,
    fragment = GL_FRAGMENT_SHADER,
};

auto load_whole_file(std::string_view path) -> std::string
{
    std::ifstream f{ path.data() };
    std::stringstream result{};
    std::string line{};

    while(std::getline(f, line)) {
        result << line << '\n';
    }

    return result.str();
}

auto load_shader_type(std::string_view path, shader_type type) -> GLuint
{
    std::string const src = load_whole_file(path);
    const char* source = src.c_str();

    GLuint shader_id = glCreateShader(static_cast<int>(type));
    GLint success = GL_FALSE;

    glShaderSource(shader_id, 1, &source, nullptr);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if(success == 0) {
        int length = 0;
        std::string msg;
        const char* shader_str = type == shader_type::vertex ? "vertex" : "fragment";

        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
        msg.resize(static_cast<std::size_t>(length));
        glGetShaderInfoLog(shader_id, length, nullptr, msg.data());
        std::cerr << "Could not compile " << shader_str << " shader: " << msg << std::endl;
    }

    return shader_id;
}

auto load_shaders(const char* vertex_file_path, const char* fragment_file_path) -> GLuint
{
    GLuint program_id = glCreateProgram();

    GLuint vs = load_shader_type(vertex_file_path, shader_type::vertex);
    GLuint fs = load_shader_type(fragment_file_path, shader_type::fragment);

    glAttachShader(program_id, vs);
    glAttachShader(program_id, fs);
    glLinkProgram(program_id);

    int success = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);

    if(success == 0) {
        std::string msg;
        int length = 0;

        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &length);
        msg.resize(static_cast<std::size_t>(length));

        glGetProgramInfoLog(program_id, length, nullptr, msg.data());
        std::cerr << "Couldn't link shader program: " << msg << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program_id;
}