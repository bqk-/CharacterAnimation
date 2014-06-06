#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include "model3d.h"
#include "rapidxml.hpp"

using namespace rapidxml;

class MeshLoader
{
private:
    static MeshLoader* m_instance;
    MeshLoader();
    Model3D* createOBJ(std::string filename);
    Model3D* createDAE(std::string filename);
    Model3D* createModel(std::string filename);

    void createSkeleton(Joint*& joint, xml_node<> *node,  std::vector<Joint*> &jointList);
    void drawSkeletonStructure(Joint* j, int i=0);


public:
    enum ModelType{NONE,OBJ,DAE};
    static MeshLoader& get();

    Model3D* openModel(std::string filename, ModelType ext=NONE);
    Model3D* createAxis(float size=10);
};

#endif // MESHLOADER_H
