#pragma once

#include <chrono>
#include <utility>

class FpsMeter
{
public:
  int Get()
  {
    if ((--m_tick) == 0)
    {
      const auto t = std::chrono::steady_clock::now();
      if ((m_measurement++) == 0) {
          m_tick = m_limit;
          m_time = t;
      }
      else {
        const auto d = std::chrono::duration_cast<std::chrono::milliseconds>(t - m_time);
        const int fps = static_cast<int>(1000. * m_limit / d.count());
        m_tick = m_limit;
        m_time = t;
        return fps;
      }
    }
    return -1;
  }

private:
    std::chrono::steady_clock::time_point m_time;
    unsigned int m_tick = 1;
    unsigned int m_measurement = 0;
    const unsigned int m_limit = 1000;
};
