//
//  Repere.cpp
//  SDL
//
//  Created by Thibault Miclo on 20/05/2014.
//  Copyright (c) 2014 in55. All rights reserved.
//

#include "Repere.h"
using namespace glm;

Repere::Repere(std::string const vertexShader, std::string const fragmentShader) : m_shader(vertexShader, fragmentShader){
    GLfloat tmp_TabVertices[]=
    {
        0,0,0,
        0,0,10,
        0,0,0,
        0,10,0,
        0,0,0,
        10,0,0
    };
    
    GLfloat tmp_TabColors[]=
    {
        1,0,0,
        1,0,0,
        0,1,0,
        0,1,0,
        0,0,1,
        0,0,1
    };
    
    for(int i(0); i < 18; i++)
    {
        g_TabVertices[i] = tmp_TabVertices[i];
        g_TabColors[i] = tmp_TabColors[i];
    }
}

Repere::~Repere(){
    //Destructor
}

void Repere::afficher(glm::mat4 &projection, glm::mat4 &modelview)
{
    glUseProgram(m_shader.getProgramID());
    
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, g_TabVertices );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, g_TabColors );
    // Envoi des matrices
    
    glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));
    

    glDrawArrays( GL_LINES, 0, 6 );
    glDisableVertexAttribArray( 1 );
    glDisableVertexAttribArray( 0 );
    
    glUseProgram(0);
}

