#include "stdafx.h"
#include "appwnd.h"
#include "globemesh.h"
#include "heightmap.h"
#include <gl/gl.h>
#include <gl/glu.h>

//
// Helpers
//

#ifdef _DEBUG
inline void TestOGL(LPCTSTR lpcszExpr, int err, LPCSTR lpcszFile, int nLine)
{
    if (err != 0)
    {
        const size_t len = 2047;
        TCHAR sz[len + 1] = { 0 };
        #pragma warning (disable : 4996)
        _sntprintf(sz, len, _T("%i: %s [%s, %i]\n"), err, lpcszExpr, lpcszFile, nLine);
        #pragma warning (default : 4996)
        OutputDebugString(sz);
    }
}
#endif

//
// Macro
//

#ifdef _DEBUG
#define VRF_OGL(expr) ((expr), TestOGL(#expr, glGetError(), __FILE__, __LINE__))
#else
#define VRF_OGL(expr) (expr)
#endif

//
// CAppWnd class
//

CAppWnd::CAppWnd(LPCSTR lpcszHeightMapPath) throw(...)
  : m_heightMapPath(lpcszHeightMapPath) // exception
  , m_callListEarth(-1)
  , m_distance(-4.0)
  , m_angle(0.0)
{
}

CAppWnd::~CAppWnd()
{
}

void CAppWnd::OnCreate()
{
    USES_CONVERSION;
    LPCTSTR lpcszHeightMapPath = CA2CT( m_heightMapPath.c_str() );

    m_callListEarth = glGenLists(1);
    VRF_OGL( glNewList(m_callListEarth, GL_COMPILE) );
    { CGlobeMesh mesh(CBitmapHeightMap(lpcszHeightMapPath, 0.04));
      mesh.DrawOpenGL(); }
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

    GLfloat light_position[4] = { -8.0f, 0.0f, 10.0f, 0.0f };
    GLfloat light_diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	
    GLfloat light_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    VRF_OGL( glLightfv(GL_LIGHT0, GL_POSITION, light_position) );
    VRF_OGL( glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse) );
    VRF_OGL( glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular) );

    Color colorAmbient(112, 146, 190);
    Color colorSpecular(16, 16, 16);
    float fShininess = 100.0f;
    VRF_OGL( glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorAmbient) );
    VRF_OGL( glMaterialfv(GL_FRONT, GL_SPECULAR, colorSpecular) );
    VRF_OGL( glMaterialf(GL_FRONT, GL_SHININESS, fShininess) );

    VRF_OGL( glEnable(GL_LIGHTING) );
    VRF_OGL( glEnable(GL_LIGHT0) );
}

void CAppWnd::OnDestroy()
{    
    VRF_OGL( glDeleteLists(m_callListEarth, 1) );
    m_callListEarth = -1;
}

void CAppWnd::OnResize(int nWidth, int nHeight)
{
    VRF_OGL( glMatrixMode(GL_PROJECTION) );
    VRF_OGL( glLoadIdentity() );
    VRF_OGL( glViewport(0, 0, nWidth, nHeight) );
    VRF_OGL( gluPerspective(45, (double)nWidth / (double)nHeight, 0.01, 200.0) );
}

void CAppWnd::OnDrawFrame()
{
    VRF_OGL( glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT) );

    VRF_OGL( glMatrixMode(GL_MODELVIEW) );
    VRF_OGL( glLoadIdentity() );

    VRF_OGL( glTranslated(0, 0, m_distance) );
    VRF_OGL( glRotated(35, 1, 0, 0) );
    VRF_OGL( glRotated(m_angle, 0, 1, 0) );
    
    VRF_OGL( glCallList(m_callListEarth) );

    VRF_OGL( glFinish() );

    m_angle += 0.3;
}
