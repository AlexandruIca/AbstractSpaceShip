#include <GL/glew.h>
#include <GL/freeglut.h>

auto render_function() -> void
{
}

auto cleanup() -> void
{
}

auto main(int argc, char *argv[]) -> int
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Utilizarea glm pentru transformari");
    glewInit();
    glutDisplayFunc(&render_function);
    glutCloseFunc(&cleanup);
    glutMainLoop();
}
