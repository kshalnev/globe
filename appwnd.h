#pragma once

#include "oglwnd.h"

//
// CAppWnd class
//

class CAppWnd : public COpenGLWnd
{
public:
    CAppWnd();
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
        case VK_HOME:
            m_distance = 4.0;
            break;
        }
    }

private:
    static void _CreateTexture(unsigned int texture, const CImage& image);
    static void _CreateCloudTexture(unsigned int texture);
    static void _CreateEarthTexture(unsigned int texture);

private:
    unsigned int m_texCloud;
    unsigned int m_texEarth;
    unsigned int m_callListEarth;
    unsigned int m_callListCloud;
    double m_angle;
    double m_distance;
};
