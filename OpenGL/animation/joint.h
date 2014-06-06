#ifndef BONE_H
#define BONE_H

#include <vector>
#include "GlFramework.h"
#include "vertexinfluence.h"
#include "keyframe.h"

class Joint
{
private:
    int m_id;                                       // Joint ID
    std::string m_name;                             // Joint Name
    GLMatrix m_matrix;                              // Joint Bind Pose Matrix (Controller)
    GLMatrix m_inverseMatrix;                       // Joint Inverse Bind Pose Matrix
    GLMatrix m_worldMatrix;                         // The World matrix
    GLMatrix m_skinningMatrix;                      // The Matrix Used for calculations (Visual Scene)
    GLMatrix m_transformMatrix;
    Joint* m_parent;                                // Parent Joint
    std::vector<Joint*> m_childs;                   // List of Joint Childs
    std::vector<Keyframe*> m_keyframes;             // List of keyframes

public:
    Joint();
    Joint(int id);
    ~Joint();

    void setId(int id) {m_id = id;}
    int getId() {return m_id;}

    void setName(std::string name) {m_name = name;}
    std::string getName() {return m_name;}

    void setBindPoseMatrix(GLMatrix matrice) {m_matrix = matrice; m_inverseMatrix = inverse(m_matrix);}
    GLMatrix getBindPoseMatrix() {return m_matrix;}

    void setInverseBindPoseMatrix(GLMatrix matrice) {m_inverseMatrix = matrice;}
    GLMatrix getInverseBindPoseMatrix() {return m_inverseMatrix;}

    void setSkinningMatrix(GLMatrix matrice) {m_skinningMatrix = matrice;}
    GLMatrix getSkinningMatrix() {return m_skinningMatrix;}

    void setTransformMatrix(GLMatrix matrice) {m_transformMatrix = matrice;}
    GLMatrix getTransformMatrix() {return m_transformMatrix;}

    void setWorldMatrix(GLMatrix matrice) {m_worldMatrix = matrice;}
    GLMatrix getWorldMatrix() {return m_worldMatrix;}

    void setParent(Joint* parent) {m_parent = parent;}
    Joint* getParent() {return m_parent;}

    void addChild(Joint* child) {m_childs.push_back(child); child->setParent(this);}
    Joint* getChild(int i) {return m_childs.at(i);}
    int getNbChild() {return m_childs.size();}

    void addKeyframe(Keyframe* keyframe) {m_keyframes.push_back(keyframe);}
    Keyframe* getKeyframe(int i) {return m_keyframes.at(i);}
    int getNbKeyframe() {return m_keyframes.size();}
};

#endif // BONE_H
