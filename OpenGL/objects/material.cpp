#include "material.h"

Material::Material()
{
}

Material::~Material()
{
}

void Material::loadColors(float* colors, int nbColors)
{
    m_nbColors = nbColors;
    m_colors = new float[nbColors*3];
    memcpy(m_colors, colors, nbColors * 3 * sizeof(float));
}

void Material::loadTextureCoordinate(float *textureCoordinates, int nbTextureCoordinates)
{
    m_nbTextureCoordinate = nbTextureCoordinates;
    m_textureCoordinate = new float[nbTextureCoordinates*2];
    memcpy(m_textureCoordinate, textureCoordinates, nbTextureCoordinates * 2 * sizeof(float));
}
