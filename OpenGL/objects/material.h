#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include <string.h>

class Material
{
private:
    float* m_colors;
    float* m_textureCoordinate;
    int m_nbColors;
    int m_nbTextureCoordinate;

public:
    Material();
    ~Material();
    void loadColors(float* colors, int nbColors);
    void loadTextureCoordinate(float* textureCoordinates, int nbTextureCoordinates);

    float* getColors() {return m_colors;}
    int getNbColors() {return m_nbColors;}
    float* getUvs() {return m_textureCoordinate;}
    int getNbUvs() {return m_nbTextureCoordinate;}

    bool hasColors() {return m_nbColors != 0;}
    bool hasTextureCoordinates() {return m_nbTextureCoordinate != 0;}
};

#endif // MATERIAL_H
