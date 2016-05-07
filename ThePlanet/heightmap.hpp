#pragma once

class IHeightMap
{
public:
  virtual ~IHeightMap() = default;
  virtual unsigned long GetWidth() const = 0;
  virtual unsigned long GetHeight() const = 0;
  virtual double GetValue(long x, long y) const = 0;
};

class DummyHeightMap : public IHeightMap
{
public:
  DummyHeightMap(unsigned long width, unsigned long height, double value = 0.0)
    : m_width(width), m_height(height), m_value(value) {}
  
  virtual unsigned long GetWidth() const override { return m_width; }
  virtual unsigned long GetHeight() const override { return m_height; }
  virtual double GetValue(long x, long y) const override { return m_value; }
  
private:
  const unsigned long m_width;
  const unsigned long m_height;
  const double m_value;
};
