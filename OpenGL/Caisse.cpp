#include "Caisse.h"


// Permet d'éviter la ré-écriture du namespace glm::

using namespace glm;


// Constructeur et Destructeur

Caisse::Caisse(float taille, std::string const vertexShader,std::string const fragmentShader, std::string const texture) : Cube(taille, vertexShader, fragmentShader),
                                                                                                                           m_texture(texture),
                                                                                                                           m_tailleCoordTextureBytes(72 * sizeof(float))
{
    // Chargement de la texture

    m_texture.charger();


    // Coordonnées de texture temporaires

    float coordTextureTmp[] = {0, 0,   1, 0,   1, 1,     // Face 1
                               0, 0,   0, 1,   1, 1,     // Face 1

                               0, 0,   1, 0,   1, 1,     // Face 2
                               0, 0,   0, 1,   1, 1,     // Face 2

                               0, 0,   1, 0,   1, 1,     // Face 3
                               0, 0,   0, 1,   1, 1,     // Face 3

                               0, 0,   1, 0,   1, 1,     // Face 4
                               0, 0,   0, 1,   1, 1,     // Face 4

                               0, 0,   1, 0,   1, 1,     // Face 5
                               0, 0,   0, 1,   1, 1,     // Face 5

                               0, 0,   1, 0,   1, 1,     // Face 6
                               0, 0,   0, 1,   1, 1};    // Face 6


    // Copie des valeurs dans le tableau final

    for(int i (0); i < 72; i++)
        m_coordTexture[i] = coordTextureTmp[i];
}


Caisse::~Caisse()
{

}


// Méthodes

void Caisse::charger()
{
    // Destruction d'un éventuel ancien VBO

    if(glIsBuffer(m_vboID) == GL_TRUE)
        glDeleteBuffers(1, &m_vboID);


    // Génération de l'ID

    glGenBuffers(1, &m_vboID);


    // Verrouillage du VBO

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);


        // Allocation de la mémoire vidéo

        glBufferData(GL_ARRAY_BUFFER, m_tailleVerticesBytes + m_tailleCoordTextureBytes, 0, GL_STATIC_DRAW);


        // Transfert des données

        glBufferSubData(GL_ARRAY_BUFFER, 0, m_tailleVerticesBytes, m_vertices);
        glBufferSubData(GL_ARRAY_BUFFER, m_tailleVerticesBytes, m_tailleCoordTextureBytes, m_coordTexture);


    // Déverrouillage de l'objet

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Destruction d'un éventuel ancien VAO

    if(glIsVertexArray(m_vaoID) == GL_TRUE)
        glDeleteVertexArrays(1, &m_vaoID);


    // Génération de l'ID du VAO

    glGenVertexArrays(1, &m_vaoID);


    // Verrouillage du VAO

    glBindVertexArray(m_vaoID);


        // Verrouillage du VBO

        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);


            // Accès aux vertices dans la mémoire vidéo

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);


            // Accès aux coordonnées de texture dans la mémoire vidéo

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(m_tailleVerticesBytes));
            glEnableVertexAttribArray(2);


        // Déverrouillage du VBO

        glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Déverrouillage du VAO

    glBindVertexArray(0);
}


void Caisse::afficher(glm::mat4 &projection, glm::mat4 &modelview)
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

            glDrawArrays(GL_TRIANGLES, 0, 36);


            // Déverrouillage de la texture

            glBindTexture(GL_TEXTURE_2D, 0);


        // Déverrouillage du VAO

        glBindVertexArray(0);


    // Désactivation du shader

    glUseProgram(0);
}
