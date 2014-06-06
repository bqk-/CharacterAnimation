#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include "object3D.h"
#include "geometry.h"
#include "material.h"

class Mesh : public Object3D
{
    private:
        unsigned int m_bufVertices;
        unsigned int m_bufColors;
        unsigned int m_bufIndices;
        unsigned int m_bufNormals;
        //unsigned int m_bufTex;

        Geometry *m_geometry;
        Material *m_material;

        std::string m_name;

        void initVBO();

    protected:
        void drawShape();

    public:
        Mesh(Geometry* g, Material *m);
        ~Mesh();

        unsigned int getBufVertices() {return m_bufVertices;}
        unsigned int getBufColors() {return m_bufColors;}
        unsigned int getBufIndices() {return m_bufIndices;}
        unsigned int getBufNormals() {return m_bufNormals;}
        //unsigned int getBufTex() {return m_bufTex;}

        Geometry* getGeometry() {return m_geometry;}
        Material* getMaterial() {return m_material;}

        void setName(std::string name) {m_name = name;}
        std::string getName() {return m_name;}

        void update();
        void updateSkinned();
};

#endif // MESH_H
