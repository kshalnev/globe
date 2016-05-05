#include "stdafx.h"
#include "globemesh.h"
#include "heightmap.h"
#include "fpsmeter.h"

#include <sstream>

#include <GLUT/glut.h>

#define VRF_OGL(expr) (expr)

double g_distance = 3.0;
double g_angle = 0.0;
CGlobeMesh g_globe(CEarthHeightMap(0.04));
FpsMeter g_fps;

/*
unsigned int g_callListEarth = 0;
*/

void init()
{
    VRF_OGL( glClearColor(0.0, 0.0, 0.0, 0.0) );

/*
    g_callListEarth = glGenLists(1);
    VRF_OGL( glNewList(g_callListEarth, GL_COMPILE) );
    g_globe.DrawOpenGL();
    VRF_OGL( glEndList() );
*/

    VRF_OGL( glEnable(GL_DEPTH_TEST) );
    VRF_OGL( glDepthFunc(GL_LESS) );

    VRF_OGL( glEnable(GL_POLYGON_SMOOTH) );
    VRF_OGL( glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) );

    VRF_OGL( glShadeModel(GL_SMOOTH) );

    VRF_OGL( glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA) );

    VRF_OGL( glAlphaFunc(GL_LESS, 0.25) );

    VRF_OGL( glDisable(GL_TEXTURE_2D) );
    VRF_OGL( glDisable(GL_ALPHA_TEST) );
    VRF_OGL( glDisable(GL_BLEND) );
    VRF_OGL( glDisable(GL_LIGHT0) );
    VRF_OGL( glDisable(GL_LIGHTING) );

    GLfloat light_position[4] = { -5.0f, 0.0f, 10.0f, 0.0f };
    GLfloat light_diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    VRF_OGL( glLightfv(GL_LIGHT0, GL_POSITION, light_position) );
    VRF_OGL( glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse) );
    VRF_OGL( glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular) );

    VRF_OGL( glEnable(GL_LIGHTING) );
    VRF_OGL( glEnable(GL_LIGHT0) );
}

/*
const unsigned int g_limit = 1000;
unsigned int g_counter = 1;
*/

void display()
{
    /*
    if ((--g_counter) == 0)
    {
        const auto t = std::chrono::steady_clock::now();
        static std::chrono::steady_clock::time_point g_clock = std::chrono::steady_clock::now();
        if (t > g_clock)
        {
            const auto d = std::chrono::duration_cast<std::chrono::milliseconds>(t - g_clock);
            const int fps = static_cast<int>(1000. * g_limit / d.count());
            std::ostringstream s;
            s << "Earth (" << fps << " FPS)";
            glutSetWindowTitle(s.str().c_str());
        }
        g_clock = t;
        g_counter = g_limit;
    }
    */

    auto fps = g_fps.Get();
    if (fps > 0) {
      std::ostringstream s;
      s << "Earth (" << fps << " FPS)";
      glutSetWindowTitle(s.str().c_str());
    }

    VRF_OGL( glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT) );

    VRF_OGL( glMatrixMode(GL_MODELVIEW) );
    VRF_OGL( glLoadIdentity() );

    VRF_OGL( glTranslated(0, 0, -g_distance) );
    VRF_OGL( glRotated(0, 1, 0, 0) );
    VRF_OGL( glRotated(g_angle, 0, 1, 0) );

    g_globe.DrawOpenGL();

/*
    VRF_OGL( glCallList(g_callListEarth) );
*/

    VRF_OGL( glFinish() );

    glutSwapBuffers();

    g_angle += 0.25;
}

void reshape(int w, int h)
{
    VRF_OGL( glMatrixMode(GL_PROJECTION) );
    VRF_OGL( glLoadIdentity() );
    VRF_OGL( glViewport(0, 0, w, h) );
    VRF_OGL( gluPerspective(45, (double)w / (double)h, 0.01, 200.0) );
    VRF_OGL( glMatrixMode(GL_MODELVIEW) );
    VRF_OGL( glLoadIdentity() );
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

    glutInitWindowSize(512, 512);

    glutInitWindowPosition(0, 0);

    glutCreateWindow("Earth");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);

    init();

    glutMainLoop();

    return 0;
}
