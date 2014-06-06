#ifndef SKELETON_H
#define SKELETON_H

#include "joint.h"
#include "GlFramework.h"

class Skeleton
{
private:
    GLMatrix m_bindShapeMatrix;     // Bind Shape Matrix
    Joint* m_rootJoint;             // Root bone
    std::vector<Joint*> m_joints;   // Joint list
    int m_nbKeyframes;              // Total number of keyframe

public:
    Skeleton();
    Skeleton(Joint* root);
    ~Skeleton();

    void setBindShapeMatrix(GLMatrix matrix) {m_bindShapeMatrix = matrix;}
    GLMatrix getBindShapeMatrix() {return m_bindShapeMatrix;}

    void setRoot(Joint* joint) {m_rootJoint = joint;}
    Joint* getRoot() {return m_rootJoint;}

    void addJoint(Joint* joint) {m_joints.push_back(joint);}
    Joint* getJoint(int i) {return m_joints.at(i);}
    int getNbJoint() {return m_joints.size();}

    void setNbKeyframe(int nbKeyframe) {m_nbKeyframes = nbKeyframe;}
    int getNbKeyframe() {return m_nbKeyframes;}
};

#endif // SKELETON_H
