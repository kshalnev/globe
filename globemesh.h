#pragma once

class IHeightMap;

class GlobeMesh
{
public:
    GlobeMesh(const IHeightMap& heightMap);
    ~GlobeMesh();

    void InitOpenGL();
    void DrawOpenGL();
    void UninitOpenGL();

private:
    struct SVertice
    {
        double vertice[3];
        double normal[3];
        double texture[2];
    };

    std::vector<SVertice> m_vertices;
    std::vector<unsigned int> m_indices;
    size_t m_countX = 0;
    size_t m_countY = 0;

    GLuint m_vb = 0;
    GLuint m_ib = 0;
};
