#pragma once

#include <windows.h>

#include <atlbase.h>
#include <atlwin.h>
#include <atlimage.h>

#include <math.h>

#include <vector>
#include <memory>
#include <exception>

#include <cassert>
#define ASSERT assert

#define VRF_HR(hr) \
    { if (FAILED((hr))) { throw std::bad_exception(); } }

#define VRF_GEN(expr) \
    { if (!(expr)) { throw std::bad_exception(); } }
