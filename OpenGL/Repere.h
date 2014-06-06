//
//  Repere.h
//  SDL
//
//  Created by Thibault Miclo on 20/05/2014.
//  Copyright (c) 2014 in55. All rights reserved.
//

#ifndef __SDL__Repere__
#define __SDL__Repere__

#include <iostream>
#include <OpenGL/gl3.h>
// Includes GLM

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

// Classe Caisse

class Repere
{
public:
    
    Repere(std::string const vertexShader, std::string const fragmentShader);
    ~Repere();
    
    void afficher(glm::mat4 &projection, glm::mat4 &modelview);
    
    
private:
    GLfloat g_TabVertices[18];
    GLfloat g_TabColors[18];
    Shader m_shader;
   
};



#endif /* defined(__SDL__Repere__) */
