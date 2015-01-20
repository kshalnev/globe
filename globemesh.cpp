#include "stdafx.h"
#include "globemesh.h"
#include "heightmap.h"

//
// Constants
//

static const double VALUE_PI = 3.1415926535897932384626433832795;
static const double VALUE_DEG_TO_RAD = VALUE_PI / 180.0;
static const double VALUE_RAD_TO_DEG = 180.0 / VALUE_PI;

//
// Macro
//

#define DEG_TO_RAD(deg) ((deg) * VALUE_DEG_TO_RAD)
#define RAD_TO_DEG(rad) ((rad) * VALUE_RAD_TO_DEG)

//
// Helpers
//

inline void _CrossProduct(const double v1[3], const double v2[3], double res[3])
{
    res[0] = v1[1] * v2[2] - v1[2] * v2[1]; // v1y * v2z - v1z * v2y
    res[1] = v1[2] * v2[0] - v1[0] * v2[2]; // v1z * v2x - v1x * v2z
    res[2] = v1[0] * v2[1] - v1[1] * v2[0]; // v1x * v2y - v1y * v2x
}

inline void _Normalize(double v[3])
{
    double len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if ( len > 0 )
    {
        v[0] /= len;
        v[1] /= len;
        v[2] /= len;
    }
}

inline void _Unproject3dPoint(const double radius, const double lonlat[2], double xyz[3])
{
    const double lon = DEG_TO_RAD(lonlat[0]);
    const double lat = DEG_TO_RAD(lonlat[1]);
    const double sin_lat = sin( lat );
    const double cos_lat = cos( lat );
    const double sin_lon = sin( lon );
    const double cos_lon = cos( lon );
    const double radius_sin_lat = radius * sin_lat;
    
    xyz[1] = radius * cos_lat;         // y
    xyz[2] = radius_sin_lat * cos_lon; // z
    xyz[0] = radius_sin_lat * sin_lon; // x
}

inline void _GetNormal(const double a[3], const double b[3], const double c[3], double n[3])
{
    const double v1[3] = { b[0]-a[0], b[1]-a[1], b[2]-a[2] };
    const double v2[3] = { c[0]-a[0], c[1]-a[1], c[2]-a[2] };
    _CrossProduct(v1, v2, n);
    _Normalize(n);
}

//
// CGlobeMesh class
//

CGlobeMesh::CGlobeMesh(const CHeightMapI& heightMap)
{
    const size_t countX = heightMap.GetWidth();
    const size_t countY = heightMap.GetHeight();
    ASSERT( countX == (2 * countY) );
    
    const double dx = 360.0 / double(countX);
    const double dy = 180.0 / double(countY);
    const double du =  1.0 / double(countX);
    const double dv = -1.0 / double(countY);
    const double gr = 1.0;
    
    std::vector<SVertice> vertices;
    vertices.resize( (countX + 1) * (countY + 1) ); // exception
    
    // init vertice coordinates and tex coordinates
    size_t ix = 0, iy = 0, idx = 0;
    double y = 0.0, v = 1.0;
    for ( iy = 0, idx = 0; iy <= countY; ++iy, y += dy, v += dv )
    {
        int elvY = (int)iy;
        if ( iy == countY )
        {
            y = 180.0;
            v = 1.0;
            elvY = 0;
        }
        
        double x = 0.0, u = 0.0;
        for ( ix = 0; ix <= countX; ++ix, x += dx, u += du, ++idx )
        {
            int elvX = (int)ix;
            if ( ix == countX )
            {
                x = 360.0;
                u = 1.0;
                elvX = 0;
            }
            
            SVertice& vertice = vertices[idx];
            
            double p[2] = { x, y };
            double elv = gr + heightMap.GetValue(elvX, elvY);
            double w[3];
            _Unproject3dPoint(elv, p, w);
            
            vertice.normal[0] = 0.0;
            vertice.normal[1] = 0.0;
            vertice.normal[2] = 0.0;
            
            vertice.texture[0] = u;
            vertice.texture[1] = v;
            
            vertice.vertice[0] = w[0];
            vertice.vertice[1] = w[1];
            vertice.vertice[2] = w[2];
        }
    }
    ASSERT(idx == vertices.size());
    
    // init normals
    for ( iy = 0; iy <= countY; ++iy )
    {
        int cy = (int)iy;
        int cyp = (int)(iy + 1);
        int cym = (int)(iy - 1);
        
        if ( cyp > (int)countY )
            cyp = 1;
        if ( cym < 0 )
            cym = (int)countY - 1;
        
        for ( ix = 0; ix <= countX; ++ix )
        {
            int cx = (int)ix;
            int cxp = (int)(ix + 1);
            int cxm = (int)(ix - 1);
            
            if ( cxp > (int)countX )
                cxp = 1;
            if ( cxm < 0 )
                cxm = (int)countX - 1;
            
            // 012
            // 345 where ix,iy points to 4
            // 678
            const SVertice& vertice0 = vertices[cxm + cym * (countX + 1)];
            const SVertice& vertice1 = vertices[cx  + cym * (countX + 1)];
            const SVertice& vertice2 = vertices[cxp + cym * (countX + 1)];
            const SVertice& vertice3 = vertices[cxm + cy  * (countX + 1)];
            const SVertice& vertice4 = vertices[cx  + cy  * (countX + 1)];
            const SVertice& vertice5 = vertices[cxp + cy  * (countX + 1)];
            const SVertice& vertice6 = vertices[cxm + cyp * (countX + 1)];
            const SVertice& vertice7 = vertices[cx  + cyp * (countX + 1)];
            const SVertice& vertice8 = vertices[cxp + cyp * (countX + 1)];
            
            // normal is summ of
            //  1    2   12   34    4   45
            // 34   45   4    6    67   7
            double n[6][3] = { 0 };
            _GetNormal(vertice4.vertice, vertice1.vertice, vertice3.vertice, n[0]);
            _GetNormal(vertice4.vertice, vertice5.vertice, vertice2.vertice, n[1]);
            _GetNormal(vertice4.vertice, vertice2.vertice, vertice1.vertice, n[2]);
            _GetNormal(vertice4.vertice, vertice3.vertice, vertice6.vertice, n[3]);
            _GetNormal(vertice4.vertice, vertice6.vertice, vertice7.vertice, n[4]);
            _GetNormal(vertice4.vertice, vertice7.vertice, vertice5.vertice, n[5]);
            
            double nn[3] = { 0 };
            for ( int t = 0; t < 6; t++ )
            {
                nn[0] += n[t][0];
                nn[1] += n[t][1];
                nn[2] += n[t][2];
            }
            _Normalize(nn);
            
            SVertice& vertice = vertices[cx + cy * (countX + 1)];
            vertice.normal[0] = nn[0];
            vertice.normal[1] = nn[1];
            vertice.normal[2] = nn[2];
        }
    }
    
    m_countX = countX;
    m_countY = countY;
    m_vertices.swap(vertices);
}

CGlobeMesh::~CGlobeMesh()
{
    m_vertices.clear();
    m_countX = 0;
    m_countY = 0;
}

void CGlobeMesh::DrawOpenGL() const
{
    glBegin(GL_TRIANGLE_STRIP);
    
    size_t iy = 0, ix = 0;
    for ( iy = 0; iy < m_countY; ++iy )
    {
        for ( ix = 0; ix <= m_countX; ++ix )
        {
            const SVertice& vertice1 = m_vertices[ix + iy * (m_countX + 1)];
            const SVertice& vertice2 = m_vertices[ix + (iy + 1) * (m_countX + 1)];
            
            glNormal3dv(vertice1.normal);
            glTexCoord2dv(vertice1.texture);
            glVertex3dv(vertice1.vertice);
            
            glNormal3dv(vertice2.normal);
            glTexCoord2dv(vertice2.texture);
            glVertex3dv(vertice2.vertice); 
        }
    }
    
    glEnd();
}