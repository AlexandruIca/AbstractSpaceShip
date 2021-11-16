#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "GL/freeglut_std.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "load_shaders.hpp"

using namespace std;

GLuint VaoId, codColLocation, myMatrixLocation;

glm::mat4 myMatrix, resizeMatrix;

int codCol;
int width = 800, height = 600;

struct spaceship_desc
{
    GLuint vertex_buffer_id = 0;
    GLuint color_buffer_id = 0;
    GLuint program_id = 0;
} spaceship;

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

    glGenBuffers(1, &spaceship.vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, spaceship.vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &spaceship.color_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, spaceship.color_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

auto destroy_vbo() -> void
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &spaceship.color_buffer_id);
    glDeleteBuffers(1, &spaceship.vertex_buffer_id);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);
}

auto create_shaders() -> void
{
    spaceship.program_id = load_shaders("spaceship_shader.vert", "spaceship_shader.frag");
    glUseProgram(spaceship.program_id);
}

auto destroy_shaders() -> void
{
    glDeleteProgram(spaceship.program_id);
}

auto initialize() -> void
{
    myMatrix = glm::mat4(1.0f);
    resizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / width, 1.f / height, 1.0));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    create_vbo();
    create_shaders();
}

auto render_function() -> void
{
    glClear(GL_COLOR_BUFFER_BIT);

    myMatrix = resizeMatrix;

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
