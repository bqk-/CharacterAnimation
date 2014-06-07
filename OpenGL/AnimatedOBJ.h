//
//  AnimatedOBJ.cpp
//  OpenGL
//
//  Created by Thibault Miclo on 30/05/2014.
//
//

#ifndef __OpenGL__AnimatedOBJ__
#define __OpenGL__AnimatedOBJ__

// Include Windows

#ifdef WIN32
#include <GL/glew.h>


// Include Mac

#elif __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>


// Include UNIX/Linux

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif
// Includes GLM

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "Shader.h"
#include "ModelOBJ.h"
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <list>

// Macro utile au VBO

#ifndef BUFFER_OFFSET

#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

class AnimatedOBJ {
public:
    AnimatedOBJ(std::list<const char*> path, std::string const vertexShader, std::string const fragmentShader, std::string const texture);
    
    void charger();
    void afficher(glm::mat4 &projection, glm::mat4 &modelview, int frame, int animation);
    void ajouterAnimation(std::list<const char*> path, std::string const vertexShader, std::string const fragmentShader, std::string const texture);
    
private:
    std::vector<glm::vec3> interpole_sec(std::vector<glm::vec3> a, std::vector<glm::vec3> b, int i, int interval);
    ModelOBJ *obj;
    std::vector< std::vector<glm::vec3> > vertices_tab;
    std::vector< std::vector< std::vector<glm::vec3> > > animations_vertices;
};

#endif /* defined(__OpenGL__ModelOBJ__) */
