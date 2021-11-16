#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <array>
#include <type_traits>
#include <vector>
#include <random>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "load_shaders.hpp"

using namespace std;

GLuint codColLocation, myMatrixLocation;

glm::mat4 myMatrix, resizeMatrix, starMatrix;

int codCol;
int width = 800, height = 600;

struct spaceship_desc
{
    GLuint vao = 0;
    GLuint vertex_buffer_id = 0;
    GLuint color_buffer_id = 0;
    GLuint program_id = 0;
} spaceship;

struct background_desc
{
    GLuint vao = 0;
    GLuint vertex_buffer_id = 0;
    GLuint program_id = 0;
} background;

constexpr int num_stars = 500;
constexpr int vertices_per_star = 6;
constexpr int floats_per_star = 36;
constexpr float star_scale_factor = 1.f / 20.f;

auto generate_star(glm::vec2 center, float const width, float const offset = 60.0f)
    -> std::array<float, floats_per_star>
{
    // clang-format off
    return std::array<float, floats_per_star>{ {
        center.x - 200.0f, center.y + 200.0f - offset, 0.0f, 1.0f, center.x, center.y,
        center.x,          center.y - 200.0f - offset, 0.0f, 1.0f, center.x, center.y,
        center.x + 200.0f, center.y + 200.0f - offset, 0.0f, 1.0f, center.x, center.y,

        center.x - 200.0f, center.y - 200.0f + offset, 0.0f, 1.0f, center.x, center.y,
        center.x,          center.y + 200.0f + offset, 0.0f, 1.0f, center.x, center.y,
        center.x + 200.0f, center.y - 200.0f + offset, 0.0f, 1.0f, center.x, center.y,
    } };
    // clang-format on
}

auto generate_background() -> std::vector<float>
{
    std::vector<float> result;
    result.reserve(num_stars * floats_per_star);

    std::mt19937 rng{ std::random_device{}() };
    std::uniform_real_distribution<float> x{ -1000.0f, 1000.0f };
    std::uniform_real_distribution<float> y{ -600.0f, 600.0f };

    for(int i = 0; i < num_stars; ++i) {
        auto star = generate_star(glm::vec2{ x(rng), y(rng) }, 200.0f);

        for(auto const f : star) {
            result.push_back(f);
        }
    }

    return result;
}

auto create_vbo() -> void
{
    // clang-format off
    GLfloat Vertices[] = {
        //spaceship body
        -200.0f, 20.0f, 0.0f, 1.0f,
        50.0f, 20.0f, 0.0f, 1.0f,
        50.0f, 70.0f, 0.0f, 1.0f,
        -200.0f, 70.0f, 0.0f, 1.0f,

        //spaceship tip
        30.0f, 10.0f, 0.0f, 1.0f,
        150.0f, 45.0f, 0.0f, 1.0f,
        30.0f, 80.0f, 0.0f, 1.0f,

        //spaceship wing1
        -210.0f, 0.0f, 0.0f, 1.0f,
        -100.0f, 20.0f, 0.0f, 1.0f,
        -190.0f, 20.0f, 0.0f, 1.0f,

        //spaceship wing2
        -190.0f, 70.0f, 0.0f, 1.0f,
        -100.0f, 70.0f, 0.0f, 1.0f,
        -210.0f, 90.0f, 0.0f, 1.0f,

        //spaceship window background
        -50.0f, 30.0f, 0.0f, 1.0f,
        10.0f, 30.0f, 0.0f, 1.0f,
        10.0f, 60.0f, 0.0f, 1.0f,
        -50.0f, 60.0f, 0.0f, 1.0f,

        //spaceship window
        -45.0f, 35.0f, 0.0f, 1.0f,
        5.0f, 35.0f, 0.0f, 1.0f,
        5.0f, 55.0f, 0.0f, 1.0f,
        -45.0f, 55.0f, 0.0f, 1.0f,

        //spaceship window sparkle
        -1.0f, 50.0f, 0.0f, 1.0f,
        2.0f, 50.0f, 0.0f, 1.0f,
        2.0f, 52.0f, 0.0f, 1.0f,
        -1.0f, 52.0f, 0.0f, 1.0f,
    };

    GLfloat Colors[] = {
        //spaceship body color
        0.5f, 0.5f, 0.5f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f,

        //spaceship tip color
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,

        //spaceship wing1 color
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,

        //spaceship wing2 color
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,

        //spaceship window background color
        0.4f, 0.4f, 0.4f, 1.0f,
        0.4f, 0.4f, 0.4f, 1.0f,
        0.4f, 0.4f, 0.4f, 1.0f,
        0.4f, 0.4f, 0.4f, 1.0f,

        //spaceship window color
        0.0f, 0.0f, 0.4f, 1.0f,
        0.0f, 0.0f, 0.4f, 1.0f,
        0.0f, 0.0f, 0.4f, 1.0f,
        0.0f, 0.0f, 0.4f, 1.0f,

        //spaceship window sparkle color
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
    };
    // clang-format on

    std::vector<float> background_verts{ generate_background() };

    glGenVertexArrays(1, &spaceship.vao);
    glBindVertexArray(spaceship.vao);

    glGenBuffers(1, &spaceship.vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, spaceship.vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &spaceship.color_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, spaceship.color_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // background
    glGenVertexArrays(1, &background.vao);
    glBindVertexArray(background.vao);

    glGenBuffers(1, &background.vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, background.vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, background_verts.size() * sizeof(float), background_verts.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));
}

auto destroy_vbo() -> void
{
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &spaceship.color_buffer_id);
    glDeleteBuffers(1, &spaceship.vertex_buffer_id);
    glDeleteBuffers(1, &background.vertex_buffer_id);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &spaceship.vao);
    glDeleteVertexArrays(1, &background.vao);
}

auto create_shaders() -> void
{
    spaceship.program_id = load_shaders("spaceship_shader.vert", "spaceship_shader.frag");
    background.program_id = load_shaders("background_shader.vert", "background_shader.frag");
}

auto destroy_shaders() -> void
{
    glDeleteProgram(spaceship.program_id);
    glDeleteProgram(background.program_id);
}

auto initialize() -> void
{
    myMatrix = glm::mat4(1.0f);
    resizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / width, 1.f / height, 1.0f));
    starMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(star_scale_factor, star_scale_factor, 1.0f));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    create_vbo();
    create_shaders();
}

auto render_function() -> void
{
    glClear(GL_COLOR_BUFFER_BIT);

    myMatrix = resizeMatrix;

    glBindVertexArray(background.vao);
    glUseProgram(background.program_id);

    myMatrixLocation = glGetUniformLocation(background.program_id, "myMatrix");
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

    myMatrixLocation = glGetUniformLocation(background.program_id, "starMatrix");
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &starMatrix[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, num_stars * vertices_per_star);

    // SPACESHIP:

    glBindVertexArray(spaceship.vao);
    glUseProgram(spaceship.program_id);

    myMatrixLocation = glGetUniformLocation(spaceship.program_id, "myMatrix");
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

    codCol = 0;
    codColLocation = glGetUniformLocation(spaceship.program_id, "codCol");
    glUniform1i(codColLocation, codCol);

    glPointSize(10.0);

    glDrawArrays(GL_POLYGON, 0, 4);
    glDrawArrays(GL_TRIANGLES, 4, 3);
    glDrawArrays(GL_TRIANGLES, 7, 3);
    glDrawArrays(GL_TRIANGLES, 10, 3);
    glDrawArrays(GL_POLYGON, 13, 4);
    glDrawArrays(GL_POLYGON, 17, 4);
    glDrawArrays(GL_POLYGON, 21, 4);

    glutPostRedisplay();
    glFlush();
}

auto cleanup() -> void
{
    destroy_shaders();
    destroy_vbo();
}

auto resize(int const w, int const h) -> void
{
    width = w;
    height = h;
    resizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / width, 1.f / height, 1.0));
}

auto main(int argc, char* argv[]) -> int
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width, height);
    glutCreateWindow("Spaceship Project");
    glewInit();
    initialize();
    glutDisplayFunc(&render_function);
    glutCloseFunc(&cleanup);
    glutReshapeFunc(&resize);
    glutMainLoop();
}
