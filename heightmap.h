#pragma once

typedef const unsigned char* (*LPF_HEIGHT_MAP)(unsigned long* pWidth, unsigned long* pHeight);

const unsigned char* GetEarthHeightMap(unsigned long* pWidth, unsigned long* pHeight);

class CHeightMapI
{
public:
  virtual ~CHeightMapI() {}
  virtual unsigned long GetWidth() const = 0;
  virtual unsigned long GetHeight() const = 0;
  virtual double GetValue(long x, long y) const = 0;
};

class CNullHeightMap : public CHeightMapI
{
public:
  CNullHeightMap(unsigned long width, unsigned long height, double value = 0) 
      : m_width(width), m_height(height), m_value(value) {}
  ~CNullHeightMap() {}
  
  virtual unsigned long GetWidth() const { return m_width; }
  virtual unsigned long GetHeight() const { return m_height; }
  virtual double GetValue(long x, long y) const 
  { 
    ASSERT( x >= 0 && x < (long)m_width && y >= 0 && y < (long)m_height );
    return m_value; 
  }

private:
  const unsigned long m_width;
  const unsigned long m_height;
  const double m_value;
};

template <LPF_HEIGHT_MAP t_pfn>
class CHeightMap : public CHeightMapI
{
public:
  CHeightMap(double exaggregation)
    : m_pHeightMap(NULL)
    , m_width(0)
    , m_height(0)
    , m_exaggregation(exaggregation * ( 1.0 / 255.0 ))
  {
    m_pHeightMap = (*t_pfn)(&m_width, &m_height);
  }
  ~CHeightMap() {}
  virtual unsigned long GetWidth() const { return m_width; }
  virtual unsigned long GetHeight() const { return m_height; }
  virtual double GetValue(long x, long y) const
  {
    ASSERT( x >= 0 && x < (long)m_width && y >= 0 && y < (long)m_height );
    return ( m_exaggregation * (double)(m_pHeightMap[y * m_width + x]) );
  }

private:
  const unsigned char* m_pHeightMap;
  unsigned long m_width;
  unsigned long m_height;
  double m_exaggregation;
};

typedef CHeightMap<GetEarthHeightMap> CEarthHeightMap;
