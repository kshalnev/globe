#pragma once

class COpenGLWnd
    : public CWindowImpl<COpenGLWnd, CWindow, CWinTraitsOR<0,0,CNullTraits> >
{
public:
    COpenGLWnd();
    ~COpenGLWnd();

    BEGIN_MSG_MAP(CAppWnd)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
    END_MSG_MAP();

    void DoIdle();

protected:
    virtual void OnCreate() {}
    virtual void OnDestroy() {}
    virtual void OnDrawFrame() {}
    virtual void OnResize(int nWidth, int nHeight) {}
    virtual void OnKeyDown(WPARAM vKey, LPARAM lParam) {}
    virtual void OnIdle() { Invalidate(FALSE); }

private:
    static HGLRC _CreateOpenGL(HDC hDC);
    static BOOL  _CloseOpenGL(HGLRC hOpenGL);
    static BOOL  _InitContext(HDC hDC, HGLRC hOpenGL);
    static BOOL  _ReleaseContext();

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

private:
    HGLRC m_hGL;
    HDC   m_hDC;
};
