#pragma once

typedef const unsigned char* (*LPF_HEIGHT_MAP)(unsigned long* pWidth, unsigned long* pHeight);

const unsigned char* GetEarthHeightMap(unsigned long* pWidth, unsigned long* pHeight);

class IHeightMap
{
public:
    virtual ~IHeightMap() {}
    virtual unsigned long GetWidth() const = 0;
    virtual unsigned long GetHeight() const = 0;
    virtual double GetValue(long x, long y) const = 0;
};

class DummyHeightMap : public IHeightMap
{
public:
    DummyHeightMap(unsigned long width, unsigned long height, double value = 0)
    : m_width(width), m_height(height), m_value(value) {}
    ~DummyHeightMap() {}

    virtual unsigned long GetWidth() const override { return m_width; }
    virtual unsigned long GetHeight() const override { return m_height; }
    virtual double GetValue(long x, long y) const override
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
class HeightMap : public IHeightMap
{
public:
    HeightMap(double exaggregation)
        : m_pHeightMap(nullptr)
        , m_width(0)
        , m_height(0)
        , m_exaggregation(exaggregation * ( 1.0 / 255.0 ))
    {
        m_pHeightMap = (*t_pfn)(&m_width, &m_height);
    }
    ~HeightMap() {}
    virtual unsigned long GetWidth() const override { return m_width; }
    virtual unsigned long GetHeight() const override { return m_height; }
    virtual double GetValue(long x, long y) const override
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

typedef HeightMap<GetEarthHeightMap> EarthHeightMap;
