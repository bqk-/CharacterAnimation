//
//  ModelOBJ.h
//  OpenGL
//
//  Created by Thibault Miclo on 20/05/2014.
//
//

#ifndef __OpenGL__ModelOBJ__
#define __OpenGL__ModelOBJ__

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
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>

// Macro utile au VBO

#ifndef BUFFER_OFFSET

#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

class ModelOBJ {
public:
    ModelOBJ(const char* path, std::string const vertexShader, std::string const fragmentShader, std::string const texture);
    ModelOBJ(std::vector<glm::vec3> verticesCoord, ModelOBJ o, std::string const vertexShader, std::string const fragmentShader, std::string const texture);
    ~ModelOBJ();
    
    void charger();
    void afficher(glm::mat4 &projection, glm::mat4 &modelview);
    void updateVBO(void *donnees, int tailleBytes, int decalage);
    std::vector<glm::vec3> getVertices();
    void* getVerticesAddr();
    void recharger(std::vector<glm::vec3> new_vertices);
    
private:
    std::vector<glm::vec3> verticesCoord,verticesNormals;
    std::vector<glm::vec2> textureCoords;
    std::vector<GLuint> vertexIndices,normalIndices,textureIndices;
    //glm::vec3 calculateNormal( const glm::vec3 &coord1, const glm::vec3 &coord2, const glm::vec3 &coord3 );
    //std::string mtlFile;
    //unsigned int nVerticesPerFace;
    
    Shader m_shader;
    
    GLuint m_vboID;
    GLuint m_vaoID;
    
    Texture m_texture;
};

#endif /* defined(__OpenGL__ModelOBJ__) */
