#include "mesh.hpp"

#include <cmath>
#include <cassert>

namespace
{

inline void CrossProduct(const float v1[3], const float v2[3], float res[3])
{
  res[0] = v1[1] * v2[2] - v1[2] * v2[1]; // v1y * v2z - v1z * v2y
  res[1] = v1[2] * v2[0] - v1[0] * v2[2]; // v1z * v2x - v1x * v2z
  res[2] = v1[0] * v2[1] - v1[1] * v2[0]; // v1x * v2y - v1y * v2x
}

inline void Normalize(float v[3])
{
  const float len = std::sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  if (len > 0)
  {
    v[0] /= len;
    v[1] /= len;
    v[2] /= len;
  }
}
  
inline void GetNormal(const float a[3], const float b[3], const float c[3], float n[3])
{
  const float v1[3] = { b[0]-a[0], b[1]-a[1], b[2]-a[2] };
  const float v2[3] = { c[0]-a[0], c[1]-a[1], c[2]-a[2] };
  CrossProduct(v1, v2, n);
  Normalize(n);
}
  
} // namespace

Mesh::Mesh()
{
}

Mesh::Mesh(IHeightMap const & heightMap)
{
  Init(heightMap);
}

void Mesh::Init(IHeightMap const & heightMap)
{
  const size_t w = heightMap.GetWidth();
  const size_t h = heightMap.GetHeight();
  
  TVerticeVector vertices;
  vertices.resize(w * h);
  
  // init vertices
  for (size_t y = 0; y < h; ++y)
  {
    for (size_t x = 0; x < w; ++x)
    {
      Vertice & v = vertices[x + y * w];
      
      v.p[0] = x;
      v.p[1] = y;
      v.p[2] = static_cast<float>(heightMap.GetValue(x, y));
      
      v.n[0] = v.n[1] = v.n[2] = 0;
    }
  }
  
  // init normals
  for (size_t y = 0; y < h; ++y)
  {
    for (size_t x = 0; x < w; ++x)
    {
      if (x == 0)
      {
        // 01  or  12
        // 2       0
        if (y == (h-1))
        {
          Vertice& vertice0 = vertices[x + y * w];
          Vertice& vertice1 = vertices[x + (y-1) * w];
          Vertice& vertice2 = vertices[(x+1) + (y-1) * w];
          GetNormal(vertice0.p, vertice1.p, vertice2.p, vertice0.n);
        }
        else
        {
          Vertice& vertice0 = vertices[x + y * w];
          Vertice& vertice1 = vertices[(x+1) + y * w];
          Vertice& vertice2 = vertices[x + (y+1) * w];
          GetNormal(vertice0.p, vertice1.p, vertice2.p, vertice0.n);
        }
      }
      else if (x == (w - 1))
      {
        // 20  or   2
        // 1       10
        if (y == (h-1))
        {
          Vertice& vertice0 = vertices[x + y * w];
          Vertice& vertice1 = vertices[(x-1) + y * w];
          Vertice& vertice2 = vertices[x + (y-1) * w];
          GetNormal(vertice0.p, vertice1.p, vertice2.p, vertice0.n);
        }
        else
        {
          Vertice& vertice0 = vertices[x + y * w];
          Vertice& vertice1 = vertices[(x-1) + (y+1) * w];
          Vertice& vertice2 = vertices[(x-1) + y * w];
          GetNormal(vertice0.p, vertice1.p, vertice2.p, vertice0.n);
        }
      }
      
      if (y == 0)
      {
        // 01  or  0
        // 2      21
        if (x == (w-1))
        {
          Vertice& vertice0 = vertices[x + y * w];
          Vertice& vertice1 = vertices[x + (y+1) * w];
          Vertice& vertice2 = vertices[(x-1) + (y+1) * w];
          GetNormal(vertice0.p, vertice1.p, vertice2.p, vertice0.n);
        }
        else
        {
          Vertice& vertice0 = vertices[x + y * w];
          Vertice& vertice1 = vertices[(x+1) + y * w];
          Vertice& vertice2 = vertices[x + (y+1) * w];
          GetNormal(vertice0.p, vertice1.p, vertice2.p, vertice0.n);
        }
      }
      else if (y == (h-1))
      {
        // 12  or   2
        // 0       10
        if (x == (w-1))
        {
          Vertice& vertice0 = vertices[x + y * w];
          Vertice& vertice1 = vertices[(x-1) + y * w];
          Vertice& vertice2 = vertices[x + (y-1) * w];
          GetNormal(vertice0.p, vertice1.p, vertice2.p, vertice0.n);
        }
        else
        {
          Vertice& vertice0 = vertices[x + y * w];
          Vertice& vertice1 = vertices[x + (y-1) * w];
          Vertice& vertice2 = vertices[(x+1) + (y-1) * w];
          GetNormal(vertice0.p, vertice1.p, vertice2.p, vertice0.n);
        }
      }
      
      if (x > 0 && x < (w-1) &&
          y > 0 && y < (h-1))
      {
        // nearest points are
        // 012
        // 345 where x,y points to 4
        // 678
        const Vertice& vertice1 = vertices[x + (y-1) * w];
        const Vertice& vertice2 = vertices[(x+1) + (y-1) * w];
        const Vertice& vertice3 = vertices[(x-1) + y * w];
              Vertice& vertice4 = vertices[x + y * w];
        const Vertice& vertice5 = vertices[(x+1) + y * w];
        const Vertice& vertice6 = vertices[(x-1) + (y+1) * w];
        const Vertice& vertice7 = vertices[x + (y+1) * w];

        // normal is summ of
        //  1    2   12   34    4   45
        // 34   45   4    6    67   7
        float n[6][3] = {};
        GetNormal(vertice4.p, vertice3.p, vertice1.p, n[0]);
        GetNormal(vertice4.p, vertice2.p, vertice5.p, n[1]);
        GetNormal(vertice4.p, vertice1.p, vertice2.p, n[2]);
        GetNormal(vertice4.p, vertice6.p, vertice3.p, n[3]);
        GetNormal(vertice4.p, vertice7.p, vertice6.p, n[4]);
        GetNormal(vertice4.p, vertice5.p, vertice7.p, n[5]);
        for (int t = 0; t < 6; t++)
        {
          vertice4.n[0] += n[t][0];
          vertice4.n[1] += n[t][1];
          vertice4.n[2] += n[t][2];
        }
        Normalize(vertice4.n);
      }
    }
  }
  
  // init indices
  TIndiceVector indices;
  indices.reserve((h-1) * (w-1) * 2 * 3);
  for (size_t y = 0; y < (h-1); ++y)
  {
    for (size_t x = 0; x < (w-1); ++x)
    {
      // 01
      // 2
      indices.push_back(static_cast<TIndiceVector::value_type>(x + y * w));
      indices.push_back(static_cast<TIndiceVector::value_type>((x+1) + y * w));
      indices.push_back(static_cast<TIndiceVector::value_type>(x + (y+1) * w));
      
      //  2
      // 10
      indices.push_back(static_cast<TIndiceVector::value_type>((x+1) + (y+1) * w));
      indices.push_back(static_cast<TIndiceVector::value_type>(x + (y+1) * w));
      indices.push_back(static_cast<TIndiceVector::value_type>((x+1) + y * w));
    }
  }

  m_vertices = std::move(vertices);
  m_indices = std::move(indices);
}
