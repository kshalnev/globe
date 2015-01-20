#include "stdafx.h"
#include "globemesh.h"
#include "heightmap.h"
#include <GLUT/glut.h>

#define VRF_OGL(expr) (expr)

double g_distance = 3.0;
double g_angle = 0.0;
unsigned int g_callListEarth = 0;

void init()
{
    g_callListEarth = glGenLists(1);
    VRF_OGL( glNewList(g_callListEarth, GL_COMPILE) );
    CGlobeMesh(CEarthHeightMap(0.04)).DrawOpenGL();
    VRF_OGL( glEndList() );
    
    VRF_OGL( glClearColor(0.0, 0.0, 0.0, 0.0) );
    
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

void display()
{
    VRF_OGL( glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT) );
    
    VRF_OGL( glMatrixMode(GL_MODELVIEW) );
    VRF_OGL( glLoadIdentity() );
    
    VRF_OGL( glTranslated(0, 0, -g_distance) );
    VRF_OGL( glRotated(0, 1, 0, 0) );
    VRF_OGL( glRotated(g_angle, 0, 1, 0) );
    
    VRF_OGL( glCallList(g_callListEarth) );
    
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