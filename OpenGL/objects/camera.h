#ifndef CAMERA_H
#define CAMERA_H

#include "GlFramework.h"
#include "quaternion.h"
#include "vector3.h"

class Camera
{
private:
    Vector3 m_position;
    Quaternion m_orientation;
    GLMatrix m_viewMatrix;
    GLMatrix m_projectionMatrix;

    float m_aspect, m_near, m_far, m_fov;

public:
    Camera();
    void translate(float x, float y, float z);
    void translateX(float shift);
    void translateY(float shift);
    void translateZ(float shift);
    void rotate(float angle, float ax, float ay, float az);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    const GLMatrix& getViewMatrix();
    void setAspectRatio(float ar);
    void setPlanes(float np, float fp);
    void setFOV(float angle);
    const GLMatrix& getProjectionMatrix();

protected:
    void buildViewMatrix();
    void buildProjectionMatrix();
};

#endif // CAMERA_H
