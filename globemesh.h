#pragma once

class CHeightMapI;

class CGlobeMesh
{
public:
    CGlobeMesh(const CHeightMapI& heightMap);
    ~CGlobeMesh();

    void DrawOpenGL();

private:
    struct SVertice
    {
        double vertice[3];
        double normal[3];
        double texture[2];
    };

    void Init();

    std::vector<SVertice> m_vertices;
    std::vector<unsigned int> m_indices;
    size_t m_countX = 0;
    size_t m_countY = 0;

    bool m_init = false;
    GLuint m_vb = 0;
    GLuint m_ib = 0;
};
