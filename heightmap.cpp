#include "stdafx.h"
#include "heightmap.h"
#include <exception>

CBitmapHeightMap::CBitmapHeightMap(LPCTSTR lpcszFilePath, double exaggregation) throw(...)
  : m_exaggregation(exaggregation * (1.0 / 255.0))
  , m_width(0)
  , m_height(0)
{
  CImage image;
  HRESULT hr = E_FAIL;
  if ( FAILED(hr = image.Load(lpcszFilePath)) )
  {
    if ( hr == E_OUTOFMEMORY )
      throw std::bad_alloc(); // exception
    else
      throw std::invalid_argument(lpcszFilePath); // exception
  }

  if ( image.GetBPP() != 8 )
    throw std::invalid_argument(lpcszFilePath); // exception

  const unsigned long width = image.GetWidth();
  const unsigned long height = image.GetHeight();
  const unsigned long size = width * height;
  const long pitch = image.GetPitch();
  const unsigned char* pSrcBytes = (const unsigned char*)image.GetBits();

  std::vector<unsigned char> values;
  values.resize(size); // exception

  unsigned char* const pDstBytes = &values[0];
  for ( unsigned long y = 0, i = 0; y < height; ++y, pSrcBytes += pitch )
  {
    for ( unsigned long x = 0; x < width; ++x, ++i )
    {
      pDstBytes[i] = pSrcBytes[x];
    }
  }

  pSrcBytes = NULL;
  image.Destroy();

  m_width = width;
  m_height = height;
  m_values.swap(values);
}

CBitmapHeightMap::~CBitmapHeightMap() throw()
{
  m_values.clear();
  m_width = 0;
  m_height = 0;
}
