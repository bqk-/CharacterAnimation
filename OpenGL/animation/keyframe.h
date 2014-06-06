#ifndef KEYFRAME_H
#define KEYFRAME_H

#include "GlFramework.h"

class Keyframe
{
private:
    float m_time;               // Time of this key frame
    GLMatrix m_transform;       // Transformation of this Key frame
    unsigned int m_interpolate; // Type of interpolation

public:
    Keyframe();
    ~Keyframe();

    void setTime(float time) {m_time = time;}
    float getTime() {return m_time;}

    void setTransformation(GLMatrix matrix) {m_transform = matrix;}
    GLMatrix getTransformation() {return m_transform;}
};

#endif // KEYFRAME_H
