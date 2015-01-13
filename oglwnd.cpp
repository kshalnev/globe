#include "stdafx.h"
#include "oglwnd.h"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

//
// COpenGLWnd class
//

COpenGLWnd::COpenGLWnd()
    : m_hDC(NULL)
    , m_hGL(NULL)
{
}

COpenGLWnd::~COpenGLWnd()
{
}

HGLRC COpenGLWnd::_CreateOpenGL(HDC hDC)
{
    ASSERT(hDC != NULL);

    if (hDC == NULL)
        return FALSE;

    HGLRC hOpenGL = NULL;

    BOOL bRes = FALSE;
    for( ; !bRes ; bRes = TRUE)
    {
        PIXELFORMATDESCRIPTOR pfd = 
        {
            sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
            1,                             // version number
            PFD_SUPPORT_OPENGL |           // support OpenGL
            PFD_DRAW_TO_WINDOW |           // draw on window
            PFD_DOUBLEBUFFER,              // double buffering mode
            PFD_TYPE_RGBA,                 // RGBA type
            32,                            // 32-bit color depth
            0, 0, 0, 0, 0, 0,              // color bits ignored
            0,                             // no alpha buffer
            0,                             // shift bit ignored
            0,                             // no accumulation buffer
            0, 0, 0, 0,                    // accum bits ignored
            16,                            // 16-bit z-buffer
            0,                             // no stencil buffer
            0,                             // no auxiliary buffer
            PFD_MAIN_PLANE,                // main layer
            0,                             // reserved
            0, 0, 0                        // layer masks ignored
        };

        INT nFmt = ::ChoosePixelFormat(hDC, &pfd);
        ASSERT(nFmt != 0);
        if (0 == nFmt)
            break;

        BOOL bPixelFmt = ::SetPixelFormat(hDC, nFmt, &pfd);
        ASSERT(bPixelFmt);
        if (FALSE == bPixelFmt)
            break;

        int nRet = ::DescribePixelFormat(hDC, nFmt, sizeof(pfd), &pfd);
        if (0 == nRet)
            break;

        if (0 == (pfd.dwFlags & PFD_DRAW_TO_WINDOW))
            break;

        hOpenGL = ::wglCreateContext(hDC);
        ASSERT(hOpenGL != NULL);
		if (NULL == hOpenGL)
			break;
    }

    ASSERT(bRes);

    return hOpenGL;
}

BOOL COpenGLWnd::_CloseOpenGL(HGLRC hOpenGL)
{
    ASSERT(hOpenGL != NULL);

    if (hOpenGL == NULL)
        return FALSE;

    ::wglMakeCurrent(NULL, NULL);
    return ::wglDeleteContext(hOpenGL);
}

BOOL COpenGLWnd::_InitContext(HDC hDC, HGLRC hOpenGL)
{
    ASSERT(hDC != NULL);
    ASSERT(hOpenGL != NULL);

    if (hDC == NULL || hOpenGL == NULL)
        return FALSE;

    ASSERT(::wglGetCurrentContext() == NULL);
    ASSERT(::wglGetCurrentDC() == NULL);

    BOOL bRes = ::wglMakeCurrent(hDC, hOpenGL); 
    DWORD dwErr = GetLastError();
    ASSERT(bRes); 

    ASSERT(::wglGetCurrentContext() == hOpenGL);
    ASSERT(::wglGetCurrentDC() == hDC);

    return bRes;
}

BOOL COpenGLWnd::_ReleaseContext()
{
    ASSERT(::wglGetCurrentContext() != NULL);
    ASSERT(::wglGetCurrentDC() != NULL);

    BOOL bRes = ::wglMakeCurrent(NULL, NULL); 
    DWORD dwErr = GetLastError();
    ASSERT(bRes); 

    ASSERT(::wglGetCurrentContext() == NULL);
    ASSERT(::wglGetCurrentDC() == NULL);

    return bRes;
}

LRESULT COpenGLWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    m_hDC = ::GetDC(m_hWnd);
    m_hGL = _CreateOpenGL(m_hDC);
    _InitContext(m_hDC, m_hGL);
    OnCreate();
    bHandled = FALSE;
    return 0;
}

LRESULT COpenGLWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    OnDestroy();
    _CloseOpenGL(m_hGL);
    m_hGL = NULL;
    ::ReleaseDC(m_hWnd, m_hDC);
    m_hDC = NULL;
    bHandled = FALSE;
    return 0;
}

LRESULT COpenGLWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    OnDrawFrame();
    SwapBuffers(m_hDC);
    bHandled = TRUE;
    return 0;
}

LRESULT COpenGLWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    RECT r;
    GetClientRect(&r);
    OnResize(r.right-r.left, r.bottom-r.top);
    bHandled = FALSE;
    return 0;
}

LRESULT COpenGLWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    OnKeyDown(wParam, lParam);
    bHandled = TRUE;
    return 0;
}

void COpenGLWnd::DoIdle()
{
    OnIdle();
}
