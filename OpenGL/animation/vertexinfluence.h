#ifndef VERTEXINFLUENCE_H
#define VERTEXINFLUENCE_H

#include <string>
#include <vector>

class VertexInfluence
{
private:
    std::vector<int> m_weights; // Weights Array
    std::vector<int> m_joints;  // Joints Array

public:
    VertexInfluence();

    void addWeight(int weight) {m_weights.push_back(weight);}
    void addJoint(int joint) {m_joints.push_back(joint);}

    int getNbInfluence() {return m_joints.size();}
    int getWeight(int i) {return m_weights.at(i);}
    int getJoint(int i) {return m_joints.at(i);}
};

#endif // VERTEXINFLUENCE_H
