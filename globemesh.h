#pragma once

class CHeightMapI;

class CGlobeMesh
{
public:
    CGlobeMesh(const CHeightMapI& heightMap);
    ~CGlobeMesh();
    
    void DrawOpenGL() const;
    
private:
    struct SVertice
    {
        double vertice[3];
        double normal[3];
        double texture[2];
    };
    
    std::vector<SVertice> m_vertices;
    size_t m_countX;
    size_t m_countY;
};