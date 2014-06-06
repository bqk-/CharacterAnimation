#include "geometry.h"

Geometry::Geometry()
{
    m_primitive = 0;
    m_nbVertices = 0;
    m_nbNormals = 0;
    m_nbIndices = 0;
}

Geometry::~Geometry()
{
}

void Geometry::loadVertices(float *vertices, int nbVertices)
{
    m_nbVertices = nbVertices;
    m_vertices = new float[nbVertices*3];
    memcpy(m_vertices, vertices, nbVertices * 3 * sizeof(float));

    loadVerticesSkinned(vertices,nbVertices);
}

void Geometry::loadNormals(float *normals, int nbNormals)
{
    m_nbNormals = nbNormals;
    m_normals = new float[nbNormals*3];
    memcpy(m_normals, normals, nbNormals * 3 * sizeof(float));

    loadNormalsSkinned(normals,nbNormals);
}

void Geometry::loadIndices(int *indices, int nbIndices)
{
    m_nbIndices = nbIndices;
    m_indices = new int[nbIndices];
    memcpy(m_indices, indices, nbIndices * sizeof(int));
}

void Geometry::loadVerticesSkinned(float *vertices, int nbVertices)
{
    m_nbVertices = nbVertices;
    m_verticesSkinned = new float[nbVertices*3];
    memcpy(m_verticesSkinned, vertices, nbVertices * 3 * sizeof(float));
}

void Geometry::loadNormalsSkinned(float *normals, int nbNormals)
{
    m_nbNormals = nbNormals;
    m_normalsSkinned = new float[nbNormals*3];
    memcpy(m_normalsSkinned, normals, nbNormals * 3 * sizeof(float));
}
