#ifndef DEF_CUBE
#define DEF_CUBE


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


// Includes

#include "Shader.h"


// Macro utile au VBO

#ifndef BUFFER_OFFSET

    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif


// Classe Cube

class Cube
{
    public:

    Cube(float taille, std::string const vertexShader, std::string const fragmentShader);
    ~Cube();

    void charger();
    void afficher(glm::mat4 &projection, glm::mat4 &modelview);
    void updateVBO(void *donnees, int tailleBytes, int decalage);


    protected:

    Shader m_shader;
    float m_vertices[108];
    float m_couleurs[108];

    GLuint m_vboID;
    int m_tailleVerticesBytes;
    int m_tailleCouleursBytes;
    GLuint m_vaoID;
};

#endif
