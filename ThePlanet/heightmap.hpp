#pragma once

#include <vector>

class IHeightMap
{
public:
  virtual ~IHeightMap() = default;
  virtual size_t GetWidth() const = 0;
  virtual size_t GetHeight() const = 0;
  virtual double GetValue(size_t x, size_t y) const = 0;
};

class DummyHeightMap : public IHeightMap
{
public:
  DummyHeightMap(size_t width, size_t height, double value = 0.0);
  
  virtual size_t GetWidth() const override { return m_width; }
  virtual size_t GetHeight() const override { return m_height; }
  virtual double GetValue(size_t x, size_t y) const override;
  
private:
  const size_t m_width;
  const size_t m_height;
  const double m_value;
};

class RandHeightMap : public IHeightMap
{
public:
  RandHeightMap(size_t width, size_t height, double maxValue = 1.0);
  
  virtual size_t GetWidth() const override { return m_width; }
  virtual size_t GetHeight() const override { return m_height; }
  virtual double GetValue(size_t x, size_t y) const override;
  
private:
  const size_t m_width;
  const size_t m_height;
  std::vector<double> m_values;
};