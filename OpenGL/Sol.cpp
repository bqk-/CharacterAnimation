#include "Sol.h"


// Permet d'Èviter la rÈ-Ècriture du namespace glm::

using namespace glm;


// Constructeur et Destructeur

Sol::Sol(float taille, std::string const vertexShader,std::string const fragmentShader, std::string const texture) : Cube(taille, vertexShader, fragmentShader),
m_texture(texture),
m_tailleCoordTextureBytes(12 * sizeof(float))
{
    // Chargement de la texture
    
    m_texture.charger();
    float tmp_vertices[]= {-taille/2, 0, -taille/2,   taille/2, 0, -taille/2,   taille/2, 0, taille/2,    // Triangle 1
        -taille/2, 0, -taille/2,   -taille/2, 0, taille/2,   taille/2, 0, taille/2};   // Triangle 2
    
    
    // CoordonnÈes de texture temporaires
    float coordTextureTmp[] = {0, 0,   7, 0,   7, 7,     // Triangle 1
        0, 0,   0, 7,   7, 7};    // Triangle 2
    
    
    // Copie des valeurs dans le tableau final
    
    for(int i (0); i < 12; i++)
        m_coordTexture[i] = coordTextureTmp[i];
    
    for(int i (0); i < 18; i++)
        m_vertices[i] = tmp_vertices[i];   // Triangle 2
}


Sol::~Sol()
{
    
}


// MÈthodes

void Sol::charger()
{
    // Destruction d'un Èventuel ancien VBO
    
    if(glIsBuffer(m_vboID) == GL_TRUE)
        glDeleteBuffers(1, &m_vboID);
    
    
    // GÈnÈration de l'ID
    
    glGenBuffers(1, &m_vboID);
    
    
    // Verrouillage du VBO
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    
    
    // Allocation de la mÈmoire vidÈo
    
    glBufferData(GL_ARRAY_BUFFER, m_tailleVerticesBytes + m_tailleCoordTextureBytes, 0, GL_STATIC_DRAW);
    
    
    // Transfert des donnÈes
    
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_tailleVerticesBytes, m_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, m_tailleVerticesBytes, m_tailleCoordTextureBytes, m_coordTexture);
    
    
    // DÈverrouillage de l'objet
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    // Destruction d'un Èventuel ancien VAO
    
    if(glIsVertexArray(m_vaoID) == GL_TRUE)
        glDeleteVertexArrays(1, &m_vaoID);
    
    
    // GÈnÈration de l'ID du VAO
    
    glGenVertexArrays(1, &m_vaoID);
    
    
    // Verrouillage du VAO
    
    glBindVertexArray(m_vaoID);
    
    
    // Verrouillage du VBO
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    
    
    // AccËs aux vertices dans la mÈmoire vidÈo
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);
    
    
    // AccËs aux coordonnÈes de texture dans la mÈmoire vidÈo
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(m_tailleVerticesBytes));
    glEnableVertexAttribArray(2);
    
    
    // DÈverrouillage du VBO
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    // DÈverrouillage du VAO
    
    glBindVertexArray(0);
}


void Sol::afficher(glm::mat4 &projection, glm::mat4 &modelview)
{
    // Activation du shader
    
    glUseProgram(m_shader.getProgramID());
    
    
    // Verrouillage du VAO
    
    glBindVertexArray(m_vaoID);
    
    
    // Envoi des matrices
    
    glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));
    
    
    // Verrouillage de la texture
    
    glBindTexture(GL_TEXTURE_2D, m_texture.getID());
    
    
    // Rendu
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    
    // DÈverrouillage de la texture
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    // DÈverrouillage du VAO
    
    glBindVertexArray(0);
    
    
    // DÈsactivation du shader
    
    glUseProgram(0);
}
