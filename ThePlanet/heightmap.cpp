#include "heightmap.hpp"

#include "perlinnoise.hpp"

#include <cassert>
#include <stdlib.h>

DummyHeightMap::DummyHeightMap(size_t width, size_t height, double value)
  : m_width(width), m_height(height), m_value(value)
{
}

double DummyHeightMap::GetValue(size_t x, size_t y) const
{
  assert(x < m_width && y < m_height);
  return m_value;
}

RandHeightMap::RandHeightMap(size_t width, size_t height, double maxValue)
  : m_width(width), m_height(height)
{
  m_values.resize(width * height);
  for (size_t i = 0, n = m_values.size(); i < n; ++i) {
      m_values[i] = maxValue * (rand() % 256) / 255.0;
  }
}

double RandHeightMap::GetValue(size_t x, size_t y) const
{
  assert(x < m_width && y < m_height);
  return m_values[x + y * m_width];
}

PerlinNoiseHeightMap::PerlinNoiseHeightMap(size_t width, size_t height)
  : m_width(width), m_height(height)
{
  m_values.resize(width * height);
  PerlinNoise noise;
  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      m_values[x + y * width] = noise.noise(x, y, 0);
    }
  }
}

double PerlinNoiseHeightMap::GetValue(size_t x, size_t y) const
{
  assert(x < m_width && y < m_height);
  return m_values[x + y * m_width];
}
