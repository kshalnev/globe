#include "stdafx.h"
#include "appwnd.h"
#include "globemesh.h"
#include "heightmap.h"
#include <gl/gl.h>
#include <gl/glu.h>

//
// Consts
//

static const TCHAR PATH_EART_TEX[] = _T("./res/earth.jpg");
static const TCHAR PATH_CLOUD_TEX[] = _T("./res/cloudmap.jpg");
static const TCHAR PATH_CLOUD_TRANS_MAP[] = _T("./res/cloudtransmap.jpg");

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

CAppWnd::CAppWnd()
    : m_texCloud(-1)
    , m_texEarth(-1)
    , m_callListEarth(-1)
    , m_callListCloud(-1)
    , m_angle(0.0)
    , m_distance(4.0)
{
}

CAppWnd::~CAppWnd()
{
}

void CAppWnd::OnCreate()
{
    VRF_OGL( glGenTextures(1, &m_texCloud) );
    _CreateCloudTexture(m_texCloud);

    VRF_OGL( glGenTextures(1, &m_texEarth) );
    _CreateEarthTexture(m_texEarth);

    m_callListEarth = glGenLists(1);
    VRF_OGL( glNewList(m_callListEarth, GL_COMPILE) );
    CGlobeMesh(CEarthHeightMap(0.04)).DrawOpenGL();
    VRF_OGL( glEndList() );

    m_callListCloud = glGenLists(1);
    VRF_OGL( glNewList(m_callListCloud, GL_COMPILE) );
    CGlobeMesh(CNullHeightMap(256, 128, 0.03)).DrawOpenGL();
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
    VRF_OGL( glEnable(GL_TEXTURE_2D) );  
}

void CAppWnd::OnDestroy()
{    
    VRF_OGL( glDeleteLists(m_callListEarth, 1) );
    VRF_OGL( glDeleteLists(m_callListCloud, 1) );
    VRF_OGL( glDeleteTextures(1, &m_texCloud) );
    VRF_OGL( glDeleteTextures(1, &m_texEarth) );
    m_texCloud = -1;
    m_texEarth = -1;
    m_callListEarth = -1;
    m_callListCloud = -1;
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

    VRF_OGL( glTranslated(0, 0, -m_distance) );
    VRF_OGL( glRotated(35, 1, 0, 0) );
    VRF_OGL( glRotated(m_angle, 0, 1, 0) );
    
    // draw earth
    VRF_OGL( glBindTexture(GL_TEXTURE_2D, m_texEarth) );
    VRF_OGL( glCallList(m_callListEarth) );

    // rotate and scale for clouds sphere
    VRF_OGL( glRotated(1.05 * m_angle, 0, 1, 0) );

    // draw clouds
    VRF_OGL( glEnable(GL_BLEND) );    
    VRF_OGL( glBindTexture(GL_TEXTURE_2D, m_texCloud) );
    VRF_OGL( glCallList(m_callListCloud) );
    VRF_OGL( glDisable(GL_BLEND) );

    VRF_OGL( glFinish() );

    m_angle += 0.3;
}

void CAppWnd::_CreateTexture(unsigned int texture, const CImage& image)
{
    ASSERT( image.GetBPP() == 32 );

    const int width = image.GetWidth();
    const int height = image.GetHeight();
    const int pitch = image.GetPitch();
    const void* pPixels = image.GetBits();
    if ( pitch < 0 )
    {
        pPixels = (const BYTE*)pPixels + (height - 1) * pitch;
    }

    VRF_OGL( glEnable(GL_TEXTURE_2D) );
	  VRF_OGL( glPixelStorei(GL_UNPACK_ALIGNMENT, 1) );
	  VRF_OGL( glBindTexture(GL_TEXTURE_2D, texture) );
    VRF_OGL( glTexImage2D( GL_TEXTURE_2D, 0, 4, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pPixels) );
	  VRF_OGL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) );
	  VRF_OGL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) );
	  VRF_OGL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
	  VRF_OGL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
	  VRF_OGL( glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPEAT) );
    VRF_OGL( glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST) );
    VRF_OGL( glDisable(GL_TEXTURE_2D) );
}

void CAppWnd::_CreateCloudTexture(unsigned int texture)
{
    CImage imageCloudTex;
    imageCloudTex.Load(PATH_CLOUD_TEX);
    ASSERT(imageCloudTex.GetBPP() == 24);

    CImage imageCloudTrans;
    imageCloudTrans.Load(PATH_CLOUD_TRANS_MAP);
    ASSERT(imageCloudTrans.GetBPP() == 8);

    ASSERT(imageCloudTex.GetWidth() == imageCloudTrans.GetWidth());
    ASSERT(imageCloudTex.GetHeight() == imageCloudTrans.GetHeight());

    CImage imageCloud;
    imageCloud.Create(imageCloudTex.GetWidth(), imageCloudTex.GetHeight(), 32);
    ASSERT(imageCloud.GetBPP() == 32);

    BYTE* pbDestBits = (BYTE*)imageCloud.GetBits();
    const BYTE* pbSrcBits = (const BYTE*)imageCloudTex.GetBits();
    const BYTE* pbAlphaBits = (const BYTE*)imageCloudTrans.GetBits();
    for ( int y = 0; y < imageCloud.GetHeight(); ++y )
    {        
        RGBQUAD* pDestRow = (RGBQUAD*)pbDestBits;        
        const RGBTRIPLE* pSrcRow = (const RGBTRIPLE*)pbSrcBits;
        const BYTE* pAlphaRow = pbAlphaBits;
        const BYTE* pAlphaRowEnd = pbAlphaBits + imageCloud.GetWidth();
        for ( ; pAlphaRow < pAlphaRowEnd; ++pSrcRow,++pDestRow,++pAlphaRow )
        {
            pDestRow->rgbBlue = pSrcRow->rgbtBlue;
            pDestRow->rgbGreen = pSrcRow->rgbtGreen;
            pDestRow->rgbRed = pSrcRow->rgbtRed;
            pDestRow->rgbReserved = *pAlphaRow;
        }

        pbSrcBits += imageCloudTex.GetPitch();
        pbAlphaBits += imageCloudTrans.GetPitch();
        pbDestBits += imageCloud.GetPitch();
    }

    imageCloudTex.Destroy();
    imageCloudTrans.Destroy();

    _CreateTexture(texture, imageCloud);

    imageCloud.Destroy();
}

void CAppWnd::_CreateEarthTexture(unsigned int texture)
{
    CImage imageOrigin;
    imageOrigin.Load(PATH_EART_TEX);
    ASSERT(imageOrigin.GetBPP() == 24);

    CImage image;
    image.Create(imageOrigin.GetWidth(), imageOrigin.GetHeight(), 32);
    ASSERT(image.GetBPP() == 32);

    imageOrigin.BitBlt(image.GetDC(), 0, 0, SRCCOPY);

    imageOrigin.Destroy();

    _CreateTexture(texture, image);

    image.ReleaseDC();
    image.Destroy();
}
