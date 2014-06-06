#include "skeleton.h"

Skeleton::Skeleton()
{
    m_rootJoint = NULL;
}

Skeleton::Skeleton(Joint *root) : m_rootJoint(root)
{
}

Skeleton::~Skeleton()
{
    delete m_rootJoint;
}
