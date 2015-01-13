#include "stdafx.h"
#include "appwnd.h"

static BOOL GetDisplaySettins(DEVMODE *pDM)
{
    ASSERT(pDM != NULL);
    return EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, pDM);
}

static BOOL ChangeDisplaySettings(DEVMODE *pDM)
{
    ASSERT(pDM != NULL);
    return ChangeDisplaySettingsEx(NULL, pDM, NULL, 0, NULL) == DISP_CHANGE_SUCCESSFUL;
}

#pragma comment(linker, "/subsystem:windows")
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    class _AppWnd : public CAppWnd
    {
    public:
      _AppWnd(LPCSTR lpcszHeightMapPath) : CAppWnd(lpcszHeightMapPath) {}
      virtual void OnFinalMessage(HWND)
      {
        ::PostQuitMessage(0);
      }
    };

    const LONG nWidth = 800;
    const LONG nHeight = 600;
    const BOOL bChangeDisplaySettings = FALSE;

    RECT r = { 0, 0, nWidth, nHeight };

    if ( lpszCmdLine == 0 || lpszCmdLine[0] == 0 )
      return -1;

    _AppWnd wnd(lpszCmdLine);
    if (HWND hWnd = wnd.Create(NULL, r, NULL, WS_POPUP))
    {
        DEVMODE dmOrigin = { 0 };
        BOOL bOrigin = bChangeDisplaySettings && GetDisplaySettins(&dmOrigin);
        BOOL bNew = FALSE;

        if (bOrigin && (dmOrigin.dmPelsWidth != nWidth || dmOrigin.dmPelsHeight != nHeight))
        {
            DEVMODE dmNew = dmOrigin;
            dmNew.dmPelsWidth = nWidth;
            dmNew.dmPelsHeight = nHeight;
            if (bNew = ChangeDisplaySettings(&dmNew))
                ::ShowCursor(FALSE);
        }

        ::ShowWindow(hWnd, SW_SHOW);

        while (::IsWindow(hWnd))
        {
            MSG msg;
            if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            else
            {
                wnd.DoIdle();
            }
        }

        if (bNew)
        {
            ChangeDisplaySettings(&dmOrigin);
            ::ShowCursor(TRUE);
        }
    }

    return 0;
}
  