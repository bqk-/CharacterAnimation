#include "model3d.h"
#include <iostream>

Model3D::Model3D()
{
    m_skeleton = NULL;
    m_name = "";
}

Model3D::Model3D(Mesh *m)
{
    m_skeleton = NULL;
    m_meshes.push_back(m);
    m_name = "";
}

Model3D::~Model3D()
{
    std::vector<Mesh*>::iterator it;
    for(it=m_meshes.begin(); it!=m_meshes.end(); it++)
    {
        delete (*it);
    }

    delete m_skeleton;
}

void Model3D::draw()
{
    std::vector<Mesh*>::iterator it;
    for(it=m_meshes.begin(); it!=m_meshes.end(); it++)
    {
        (*it)->draw();
    }
}

void Model3D::setupSkeleton(int iFrame)
{
    // TODO: Change values by animation system variables
    float animationTime = 0.0f;
    float totalTime = 1.0f;

    // Initialize root bone
    m_skeleton->getRoot()->setWorldMatrix(m_skeleton->getRoot()->getBindPoseMatrix());

    // Update the tree of bones
    static QStack<Joint*> bonesStack;
    bonesStack.push(m_skeleton->getRoot());

    if(iFrame == -1)
    {
        // Treat all bones
        while(!bonesStack.empty())
        {
            Joint *curBone = bonesStack.pop();
            GLMatrix bindPoseMatrix = curBone->getBindPoseMatrix(); //m_JointMatrix

            // Calculate the World matrix for current bone
            if(curBone->getParent())
                curBone->setWorldMatrix(bindPoseMatrix * curBone->getParent()->getWorldMatrix());

            // Handle its Children
            if(curBone->getNbChild() != 0)
            {
                int nbChildren = curBone->getNbChild();
                for (int iIndex=0; iIndex<nbChildren; iIndex++)
                {
                    bonesStack.push(curBone->getChild(iIndex));
                }
            }
        } // End While
    }
    else
    {
        // Treat all bones
        while(!bonesStack.empty())
        {
            Joint *curBone = bonesStack.pop();
            GLMatrix worldMatrix = curBone->getWorldMatrix();

            // Interpolation
            if(curBone->getNbKeyframe() > 0/* && frame > 0*/)
            {
                Q_ASSERT_X(iFrame < curBone->getNbKeyframe(), "setupSkeleton", "Invalid Key frame");

                float inBetween = (float)(animationTime * (float)m_skeleton->getNbKeyframe() / totalTime);
                inBetween -= iFrame;

                if (iFrame <  curBone->getNbKeyframe() - 1)
                {
                    worldMatrix = interpolate(curBone->getKeyframe(iFrame)->getTransformation(),
                                              curBone->getKeyframe(iFrame + 1)->getTransformation(),
                                              inBetween);
                }
                else
                {
                    worldMatrix = curBone->getKeyframe(iFrame)->getTransformation();
                }
            }

            // Calculate the World matrix for current bone
            if(curBone->getParent())
                worldMatrix = curBone->getParent()->getWorldMatrix() * curBone->getBindPoseMatrix();

            curBone->setWorldMatrix(worldMatrix);
            curBone->setSkinningMatrix(curBone->getInverseBindPoseMatrix() * worldMatrix);

            // Handle its Children
            if(curBone->getNbChild() != 0)
            {
                int nbChildren = curBone->getNbChild();
                for (int iIndex=0; iIndex<nbChildren; iIndex++)
                {
                    bonesStack.push(curBone->getChild(iIndex));
                }
            }
        } // End While
    }

    std::cout << "setup skeleton: \t OK" << std::endl;
}

void Model3D::setupBindPose()
{
    Geometry *geo = getMesh()->getGeometry();
    int nbVertices = geo->getNbVertices();

    std::vector<float> vert;
    std::vector<float> norm;

    for (int iVertex=0; iVertex<nbVertices; iVertex++)
    {
        // Vertex
        Vector3 Vertex(geo->getVertice(iVertex*3), geo->getVertice(iVertex*3+1), geo->getVertice(iVertex*3+2));
        Vector3 tmpVertex = Vertex;

        // Normal
        Vector3 Normal(geo->getNormal(iVertex*3), geo->getNormal(iVertex*3+1), geo->getNormal(iVertex*3+2));
        Vector3 tmpNormal;
        Vector3 tmpNormalTransform;

        //
        float totalJointsWeight = 0.0f;
        float normalizedWeight = 0.0f;

        for (int iInfluence=0; iInfluence<geo->getVertexInfluence(iVertex)->getNbInfluence(); iInfluence++)
        {
            /*
            v += {[(v * BSM) * IBMi * JMi] * JW}
            • n: The number of joints that influence vertex v
            • BSM: Bind-shape matrix
            • IBMi: Inverse bind-pose matrix of joint i
            • JMi: Transformation matrix of joint i
            • JW: Weight of the influence of joint i on vertex v
            */

            Joint *iJoint = m_skeleton->getJoint(geo->getVertexInfluence(iVertex)->getJoint(iInfluence));
            GLMatrix BSM = m_skeleton->getBindShapeMatrix();
            GLMatrix IBMI = iJoint->getInverseBindPoseMatrix();
            GLMatrix JMi = iJoint->getBindPoseMatrix();
            float JW = geo->getVertexWeight(geo->getVertexInfluence(iVertex)->getWeight(iInfluence));
            //tmpVertex += (((Vertex * BSM) * IBMI * JMi) * JW);
            tmpVertex += (((Vertex * BSM) * IBMI * JMi) * JW);

            // TODO: Rotate vector function
            //m_skeleton->getJoint(geo->getVertexInfluence(iVertex)->getJoint(iInfluence))->getSkinningMatrix()->RotateVector(Normal,tmpNormalTransform);
            tmpNormal  += tmpNormalTransform * geo->getVertexWeight(geo->getVertexInfluence(iVertex)->getWeight(iInfluence));
            totalJointsWeight += JW;
        }

        // Normalized values
        if (totalJointsWeight != 1.0f && totalJointsWeight != 0.0f)
        {
            normalizedWeight = 1.0f / totalJointsWeight;
            //tmpVertex *= normalizedWeight;
            tmpNormal *= normalizedWeight;
        }

        // Add calculated values to list
        vert.push_back(tmpVertex.X());
        vert.push_back(tmpVertex.Y());
        vert.push_back(tmpVertex.Z());

        norm.push_back(tmpNormal.X());
        norm.push_back(tmpNormal.Y());
        norm.push_back(tmpNormal.Z());
    }

    // Loas new data
    geo->loadVerticesSkinned(vert.data(),vert.size()/3);
    geo->loadNormalsSkinned(norm.data(),norm.size()/3);

    std::cout << "setup bind pose: \t OK" << std::endl;
}

float Model3D::interpolate(float v1, float v2, float t)
{
    return ((v2 * t) + ((1 - t) * v1));
}

GLMatrix Model3D::interpolate(GLMatrix v1, GLMatrix v2, float t)
{
    GLMatrix mat;

    for(int i=0; i<16; i++)
        mat.data[i] = interpolate(v1.data[i],v2.data[i],t);

    return mat;
}
