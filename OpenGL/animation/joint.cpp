#include "joint.h"

Joint::Joint() : m_id(-1), m_name("")
{
    m_matrix.setIdentity();
    m_inverseMatrix.setIdentity();
    m_skinningMatrix.setIdentity();
    m_worldMatrix.setIdentity();
    m_parent = NULL;
}

Joint::Joint(int id) : m_id(id), m_name("")
{
    m_matrix.setIdentity();
    m_inverseMatrix.setIdentity();
    m_skinningMatrix.setIdentity();
    m_worldMatrix.setIdentity();
    m_parent = NULL;
}

Joint::~Joint()
{
    // Free Joint Child list
    std::vector<Joint*>::iterator it;
    for(it=m_childs.begin(); it!=m_childs.end(); it++)
    {
        delete (*it);
    }

    // Free Keyframe list
    std::vector<Keyframe*>::iterator it2;
    for(it2=m_keyframes.begin(); it2!=m_keyframes.end(); it2++)
    {
        delete (*it2);
    }
}
