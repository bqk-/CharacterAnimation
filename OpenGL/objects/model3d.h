#ifndef MODEL3D_H
#define MODEL3D_H

#include <QStack>
#include "mesh.h"
#include "skeleton.h"
#include "vector3.h"

class Model3D
{
private:
    std::vector<Mesh*> m_meshes;
    Skeleton* m_skeleton;
    std::string m_name;

    float interpolate(float v1, float v2, float t);
    GLMatrix interpolate(GLMatrix v1, GLMatrix v2, float t);

public:
    Model3D();
    Model3D(Mesh* m);
    ~Model3D();

    void addMesh(Mesh* mesh) {m_meshes.push_back(mesh);}
    Mesh* getMesh(int i=0) {return m_meshes.at(i);}

    void setSkeleton(Skeleton* skeleton) {m_skeleton = skeleton;}
    Skeleton* getSkeleton() {return m_skeleton;}

    void setName(std::string name) {m_name = name;}
    std::string getName() {return m_name;}

    void draw();
    void setupSkeleton(int iFrame=-1);
    void setupBindPose();
};

#endif // MODEL3D_H
