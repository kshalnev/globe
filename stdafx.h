#pragma once

#include <math.h>

#include <vector>
#include <memory>
#include <exception>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <cassert>
#define ASSERT assert

#define VRF_HR(hr) \
{ if (FAILED((hr))) { throw std::bad_exception(); } }

#define VRF_GEN(expr) \
{ if (!(expr)) { throw std::bad_exception(); } }
