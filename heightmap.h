#pragma once

class CHeightMapI
{
public:
  virtual ~CHeightMapI() throw() {}
  virtual unsigned long GetWidth() const throw() = 0;
  virtual unsigned long GetHeight() const throw() = 0;
  virtual double GetValue(long x, long y) const throw() = 0;
};

class CNullHeightMap : public CHeightMapI
{
public:
  CNullHeightMap(unsigned long width, unsigned long height, double value = 0.0) throw()
      : m_width(width), m_height(height), m_value(value) {}
  ~CNullHeightMap() throw() {}
  
  virtual unsigned long GetWidth() const throw() { return m_width; }
  virtual unsigned long GetHeight() const throw() { return m_height; }
  virtual double GetValue(long x, long y) const throw()
  { 
    ASSERT( x >= 0 && x < (long)m_width && y >= 0 && y < (long)m_height );
    return m_value; 
  }

private:
  const unsigned long m_width;
  const unsigned long m_height;
  const double m_value;
};

class CBitmapHeightMap : public CHeightMapI
{
public:
  CBitmapHeightMap(LPCTSTR lpcszFilePath, double exaggregation) throw(...); // exception
  ~CBitmapHeightMap() throw();

  virtual unsigned long GetWidth() const throw() { return m_width; }
  virtual unsigned long GetHeight() const throw() { return m_height; }
  virtual double GetValue(long x, long y) const throw()
  {
    ASSERT( x >= 0 && x < (long)m_width && y >= 0 && y < (long)m_height );
    return ( m_exaggregation * (double)(m_values[y * m_width + x]) );
  }

private:
  const double m_exaggregation;
  std::vector<unsigned char> m_values;
  unsigned long m_width;
  unsigned long m_height;
};
