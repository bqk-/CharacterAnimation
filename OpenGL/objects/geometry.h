#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <string.h>
#include "vertexinfluence.h"

class Geometry
{
private:
    float* m_vertices;
    float* m_verticesSkinned;
    float* m_normals;
    float* m_normalsSkinned;
    int* m_indices;
    std::vector<float> m_vertexWeight;
    std::vector<VertexInfluence*> m_vertexInfluence;

    int m_nbVertices;
    int m_nbNormals;
    int m_nbIndices;

    unsigned int m_primitive;

public:
    Geometry();
    ~Geometry();

    void loadVertices(float* vertices, int nbVertices);
    void loadVerticesSkinned(float* vertices, int nbVertices);
    void loadNormals(float* normals, int nbNormals);
    void loadNormalsSkinned(float* normals, int nbNormals);
    void loadIndices(int* indices, int nbIndices);

    void setVertice(int id, float x, float y, float z) {if(m_nbVertices==0) return; m_vertices[id*3] = x; m_vertices[id*3+1] = y; m_vertices[id*3+2] = z;}
    void setVerticeSkinned(int id, float x, float y, float z) {if(m_nbVertices==0) return; m_verticesSkinned[id*3] = x; m_verticesSkinned[id*3+1] = y; m_verticesSkinned[id*3+2] = z;}
    void setNormal(int id, float nx, float ny, float nz) {if(m_nbNormals==0) return; m_normals[id*3] = nx; m_normals[id*3+1] = ny; m_normals[id*3+2] = nz;}
    void setNormalSkinned(int id, float nx, float ny, float nz) {if(m_nbNormals==0) return; m_normalsSkinned[id*3] = nx; m_normalsSkinned[id*3+1] = ny; m_normalsSkinned[id*3+2] = nz;}
    void setIndice(int id, float v) {if(m_nbIndices==0) return; m_indices[id] = v;}

    float getVertice(int id) {return m_vertices[id];}
    float getVerticeSkinned(int id) {return m_verticesSkinned[id];}
    float getNormal(int id) {return m_normals[id];}
    float getNormalSkinned(int id) {return m_normalsSkinned[id];}

    float* getVertices() {return m_vertices;}
    float* getNormals() {return m_normals;}
    float* getVerticesSkinned() {return m_verticesSkinned;}
    float* getNormalsSkinned() {return m_normalsSkinned;}
    int* getIndices() {return m_indices;}

    void addVertexInfluence(VertexInfluence* vertexInfluence){m_vertexInfluence.push_back(vertexInfluence);}
    VertexInfluence* getVertexInfluence(int i) {return m_vertexInfluence.at(i);}
    int getNbVertexInfluence() {return m_vertexInfluence.size();}

    void addVertexWeight(float vertexWeight) {m_vertexWeight.push_back(vertexWeight);}
    float getVertexWeight(int i) {return m_vertexWeight.at(i);}
    int getNbVertexWeight() {return m_vertexWeight.size();}

    int getNbVertices() {return m_nbVertices;}
    int getNbNormals() {return m_nbNormals;}
    int getNbIndices() {return m_nbIndices;}

    unsigned int getPrimitive() {return m_primitive;}
    void setPrimitive(unsigned int p) {m_primitive = p;}

    bool hasVertices() {return m_nbVertices != 0;}
    bool hasIndices() {return m_nbIndices != 0;}
    bool hasNormals() {return m_nbNormals != 0;}
};

#endif // GEOMETRY_H
