#include "camera.h"

Camera::Camera()
{
}

void Camera::translate(float x, float y, float z)
{
    translateX(x);
    translateY(y);
    translateZ(z);
}

void Camera::translateX(float shift)
{
    m_position = m_orientation * Vector3::xAxis() /* * m_orientation.conjuguate()*/ * shift;
}

void Camera::translateY(float shift)
{
    m_position = m_orientation * Vector3::yAxis() * shift;
}

void Camera::translateZ(float shift)
{
    m_position = m_orientation * Vector3::zAxis() * shift;
}

void Camera::rotate(float angle, float ax, float ay, float az)
{
    if(ax != 0.f) rotateX(angle);
    if(ay != 0.f) rotateY(angle);
    if(az != 0.f) rotateZ(angle);
}

void Camera::rotateX(float angle)
{
    Quaternion nrot(Vector3(1.0f, 0.0f, 0.0f), angle * (M_PI/180));
    m_orientation = m_orientation * nrot;
}

void Camera::rotateY(float angle)
{
    Quaternion nrot(Vector3(0.0f, 1.0f, 0.0f), angle * (M_PI/180));
    m_orientation = m_orientation * nrot;
}

void Camera::rotateZ(float angle)
{
    Quaternion nrot(Vector3(0.0f, 0.0f, 1.0f), angle * (M_PI/180));
    m_orientation = m_orientation * nrot;
}

void Camera::setAspectRatio(float ar) //Hauteur/Largeur
{
    m_aspect = ar;

    //Build view matrix V
    buildViewMatrix();

    //Buil projection matrix P
    buildProjectionMatrix();
}

void Camera::setPlanes(float np, float fp) //Near & Far
{
    //Near = 1/tan(angle chp. vision horizontal/2)
    m_near = np;
    m_far = fp;

    //Build view matrix V
    buildViewMatrix();

    //Buil projection matrix P
    buildProjectionMatrix();
}

void Camera::setFOV(float angle) //Angle d'ouverture de la camera
{
    m_fov = angle;

    //Build view matrix V
    buildViewMatrix();

    //Buil projection matrix P
    buildProjectionMatrix();
}

const GLMatrix& Camera::getViewMatrix()
{
    return m_viewMatrix;
}

const GLMatrix& Camera::getProjectionMatrix()
{
    return m_projectionMatrix;
}

void Camera::buildViewMatrix()
{
    //Créer à partir de l'orientation du quaternion et la position de la camera
    /*---------*\
     |  R  -RP  |
     |  0   1   |
     \*--------*/

    float* R = m_orientation.getRotationMatrix();
    //Vector3 RP = R * m_position;

    /*
    m_viewMatrix.m[0][0] = R[0]; m_viewMatrix.m[0][1] = R[1]; m_viewMatrix.m[0][2] = R[2]; m_viewMatrix.m[0][3] = -RP.Z();
    m_viewMatrix.m[1][0] = R[3]; m_viewMatrix.m[1][1] = R[4]; m_viewMatrix.m[1][2] = R[5]; m_viewMatrix.m[1][3] = -RP.Y();
    m_viewMatrix.m[2][0] = R[6]; m_viewMatrix.m[2][1] = R[7]; m_viewMatrix.m[2][2] = R[8]; m_viewMatrix.m[2][3] = -RP.X();
    m_viewMatrix.m[3][0] = 0.0f; m_viewMatrix.m[3][1] = 0.0f; m_viewMatrix.m[3][2] = 0.0f; m_viewMatrix.m[3][3] = 1.0f;
    */
}

void Camera::buildProjectionMatrix()
{
    float Sx, Sy, Sz, Dx, Dy, Pz;
    float range = tan(m_fov*(M_PI/180)/2)*m_near;
    float left = -range*m_aspect;
    float right = range*m_aspect;
    float top = range;
    float bottom = -range;

    /*
    Sx = (2*m_near)/(range*m_aspect + range*m_aspect);
    Sy = m_near/range;
    Sz = -(m_far+m_near)/(m_far-m_near);
    Pz = -(2*m_far*m_near)/(m_far-m_near);
    */

    Sx = (2*m_near)/(range-left);
    Sy = (2*m_near)/(top-bottom);
    Sz = -(m_far+m_near)/(m_far-m_near);
    Dx = (right+left)/(right-left);
    Dy = (top+bottom)/(top-bottom);
    Pz = -(2*m_far*m_near)/(m_far-m_near);

    m_projectionMatrix.m[0][0] = Sx;   m_projectionMatrix.m[0][1] = 0.0f; m_projectionMatrix.m[0][2] = Dx; m_projectionMatrix.m[0][3] = 0.0f;
    m_projectionMatrix.m[1][0] = 0.0f; m_projectionMatrix.m[1][1] = Sy;   m_projectionMatrix.m[1][2] = Dy; m_projectionMatrix.m[1][3] = 0.0f;
    m_projectionMatrix.m[2][0] = 0.0f; m_projectionMatrix.m[2][1] = 0.0f; m_projectionMatrix.m[2][2] = Sz;   m_projectionMatrix.m[2][3] = Pz;
    m_projectionMatrix.m[3][0] = 0.0f; m_projectionMatrix.m[3][1] = 0.0f; m_projectionMatrix.m[3][2] =-1.0f; m_projectionMatrix.m[3][3] = 1.0f;
}
