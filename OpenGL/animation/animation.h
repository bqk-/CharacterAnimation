#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "keyframe.h"

class Animation
{
private:
    std::vector<Keyframe*> m_keyframes;

public:
    Animation();
    int getNbKeyframes() {return m_keyframes.size();}
};

#endif // ANIMATION_H
