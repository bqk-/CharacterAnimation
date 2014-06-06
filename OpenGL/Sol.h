//
//  Sol.h
//  OpenGL
//
//  Created by Thibault Miclo on 20/05/2014.
//
//

#ifndef __OpenGL__Sol__
#define __OpenGL__Sol__


// Includes

#include "Cube.h"
#include "Texture.h"
#include <string>


// Classe Caisse

class Sol : public Cube
{
public:
    
    Sol(float taille, std::string const vertexShader, std::string const fragmentShader, std::string const texture);
    ~Sol();
    
    void charger();
    void afficher(glm::mat4 &projection, glm::mat4 &modelview);
    
    
private:
    
    Texture m_texture;
    float m_coordTexture[12];
    int m_tailleCoordTextureBytes;
};



#endif /* defined(__OpenGL__Sol__) */
