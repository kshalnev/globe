#pragma once

#include "heightmap.hpp"

#include <vector>

class Mesh
{
public:
  struct Vertice
  {
    float p[3]; // point xyz
    float n[3]; // normal xyz
  };
  
  using TVerticeVector = std::vector<Vertice>;
  using TIndiceVector = std::vector<uint32_t>;
  
  Mesh();
  Mesh(IHeightMap const & heightMap);
  
  void Init(IHeightMap const & heightMap);
  
  TVerticeVector const & Vertices() const { return m_vertices; }
  TIndiceVector const & Indices() const { return m_indices; }
  
  void const * VerticesData() const { return m_vertices.data(); }
  size_t VerticesSize() const {
    return sizeof(TVerticeVector::value_type) * m_vertices.size();
  }

  void const * IndicesData() const { return m_indices.data(); }
  size_t IndicesSize() const {
    return sizeof(TIndiceVector::value_type) * m_indices.size();
  }
    
private:
  TVerticeVector m_vertices;
  TIndiceVector m_indices;
};
