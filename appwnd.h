#pragma once

#include "oglwnd.h"

class Color
{
public:
  Color(BYTE r = 0, BYTE g = 0, BYTE b = 0, BYTE a = 0)
  {
    m_color[0] = (float)r / 255.0f;
    m_color[1] = (float)g / 255.0f;
    m_color[2] = (float)b / 255.0f;
    m_color[3] = 1.0;
  }
  Color(COLORREF color)
  {
    BYTE r = LOBYTE(LOWORD(color));
    BYTE g = HIBYTE(LOWORD(color));
    BYTE b = LOBYTE(HIWORD(color));
    BYTE a = HIBYTE(HIWORD(color));
    m_color[0] = (float)r / 255.0f;
    m_color[1] = (float)g / 255.0f;
    m_color[2] = (float)b / 255.0f;
    m_color[3] = 1.0;
  }
  operator const float* () const
  {
    return m_color;
  }
private:
  float m_color[4];
};

//
// CAppWnd class
//

class CAppWnd : public COpenGLWnd
{
public:
    CAppWnd(LPCSTR lpcszHeightMapPath) throw(...);
    ~CAppWnd();

protected:
    virtual void OnCreate();
    virtual void OnDestroy();
    virtual void OnDrawFrame();
    virtual void OnResize(int nWidth, int nHeight);
    virtual void OnKeyDown(WPARAM vKey, LPARAM lParam) 
    {
        switch (vKey)
        {
        case VK_ESCAPE:
            DestroyWindow();
            break;
        case VK_UP:
            m_distance -= 0.05;
            break;
        case VK_DOWN:
            m_distance += 0.05;
            break;
        }
    }

private:
    const std::string m_heightMapPath;
    unsigned int m_callListEarth;
    double m_distance;
    double m_angle;
};
