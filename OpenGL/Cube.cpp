#include "Cube.h"


// Permet d'éviter la ré-écriture du namespace glm::

using namespace glm;


// Constructeur et Destructeur

Cube::Cube(float taille, std::string const vertexShader, std::string const fragmentShader) : m_shader(vertexShader, fragmentShader), m_vboID(0),
                                                                                             m_tailleVerticesBytes(108 * sizeof(float)),
                                                                                             m_tailleCouleursBytes(108 * sizeof(float)), m_vaoID(0)
{
    // Chargement du shader

    m_shader.charger();


    // Division de la taille

    taille /= 2;


    // Vertices temporaires

    float verticesTmp[] = {-taille, -taille, -taille,   taille, -taille, -taille,   taille, taille, -taille,     // Face 1
                           -taille, -taille, -taille,   -taille, taille, -taille,   taille, taille, -taille,     // Face 1

                           taille, -taille, taille,   taille, -taille, -taille,   taille, taille, -taille,       // Face 2
                           taille, -taille, taille,   taille, taille, taille,   taille, taille, -taille,         // Face 2

                           -taille, -taille, taille,   taille, -taille, taille,   taille, -taille, -taille,      // Face 3
                           -taille, -taille, taille,   -taille, -taille, -taille,   taille, -taille, -taille,    // Face 3

                           -taille, -taille, taille,   taille, -taille, taille,   taille, taille, taille,        // Face 4
                           -taille, -taille, taille,   -taille, taille, taille,   taille, taille, taille,        // Face 4

                           -taille, -taille, -taille,   -taille, -taille, taille,   -taille, taille, taille,     // Face 5
                           -taille, -taille, -taille,   -taille, taille, -taille,   -taille, taille, taille,     // Face 5

                           -taille, taille, taille,   taille, taille, taille,   taille, taille, -taille,         // Face 6
                           -taille, taille, taille,   -taille, taille, -taille,   taille, taille, -taille};      // Face 6


    // Couleurs temporaires

    float couleursTmp[] = {1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 1
                           1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 1

                           0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 2
                           0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 2

                           0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,           // Face 3
                           0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,           // Face 3

                           1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 4
                           1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 4

                           0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 5
                           0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 5

                           0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,           // Face 6
                           0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0};          // Face 6


    // Copie des valeurs dans les tableaux finaux

    for(int i(0); i < 108; i++)
    {
        m_vertices[i] = verticesTmp[i];
        m_couleurs[i] = couleursTmp[i];
    }
}


Cube::~Cube()
{
    // Destruction du VBO

    glDeleteBuffers(1, &m_vboID);


    // Destruction du VAO

    glDeleteVertexArrays(1, &m_vaoID);
}


// Méthodes

void Cube::charger()
{
    // Destruction d'un éventuel ancien VBO

    if(glIsBuffer(m_vboID) == GL_TRUE)
        glDeleteBuffers(1, &m_vboID);


    // Génération de l'ID

    glGenBuffers(1, &m_vboID);


    // Verrouillage du VBO

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);


        // Allocation de la mémoire vidéo

        glBufferData(GL_ARRAY_BUFFER, m_tailleVerticesBytes + m_tailleCouleursBytes, 0, GL_STATIC_DRAW);


        // Transfert des données

        glBufferSubData(GL_ARRAY_BUFFER, 0, m_tailleVerticesBytes, m_vertices);
        glBufferSubData(GL_ARRAY_BUFFER, m_tailleVerticesBytes, m_tailleCouleursBytes, m_couleurs);


    // Déverrouillage de l'objet

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Destruction d'un éventuel ancien VAO

    if(glIsVertexArray(m_vaoID) == GL_TRUE)
        glDeleteVertexArrays(1, &m_vaoID);


    // Génération de l'identifiant du VAO

    glGenVertexArrays(1, &m_vaoID);


    // Verrouillage du VAO

    glBindVertexArray(m_vaoID);


        // Verrouillage du VBO

        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);


            // Accès aux vertices dans la mémoire vidéo

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);


            // Accès aux couleurs dans la mémoire vidéo

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(m_tailleVerticesBytes));
            glEnableVertexAttribArray(1);


        // Déverrouillage du VBO

        glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Déverrouillage du VAO

    glBindVertexArray(0);
}


void Cube::afficher(glm::mat4 &projection, glm::mat4 &modelview)
{
    // Activation du shader

    glUseProgram(m_shader.getProgramID());


        // Verrouillage du VAO

        glBindVertexArray(m_vaoID);


            // Envoi des matrices

            glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));


            // Rendu

            glDrawArrays(GL_TRIANGLES, 0, 36);


        // Déverrouillage du VAO

        glBindVertexArray(0);


    // Désactivation du shader

    glUseProgram(0);
}


void Cube::updateVBO(void *donnees, int tailleBytes, int decalage)
{
    // Verrouillage du VBO

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);


        // Récupération de l'adresse du VBO

        void *adresseVBO = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);


        // Si l'adresse retournée est nulle alors on arrête le transfert

        if(adresseVBO == NULL)
        {
            std::cout << "Erreur au niveau de la récupération du VBO" << std::endl;
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            return;
        }


        // Mise à jour des données

        memcpy((char*)adresseVBO + decalage, donnees, tailleBytes);


        // Annulation du pointeur

        glUnmapBuffer(GL_ARRAY_BUFFER);
        adresseVBO = 0;


    // Déverrouillage du VBO

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
