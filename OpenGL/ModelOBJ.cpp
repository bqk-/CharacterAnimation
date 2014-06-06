//
//  ModelOBJ.cpp
//  OpenGL
//
//  Created by Thibault Miclo on 20/05/2014.
//
//

#include "ModelOBJ.h"

ModelOBJ::ModelOBJ(const char* path, std::string const vertexShader, std::string const fragmentShader, std::string const texture) : m_shader(vertexShader, fragmentShader){
    m_texture = texture;
    m_shader.charger();
    
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;

    FILE * file = fopen(path, "r");
    if( file == NULL ){
        std::cout << "Fopen failed for path : " << path << std::endl;
    }
    
    while( 1 ){
        
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.
        
        // else : parse lineHeader
        if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        }else if ( strcmp( lineHeader, "f" ) == 0 ){
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3] );
            if (matches != 12){
                //std::cout << "File can't be read by this simple parser : ( Try exporting with other options )" << std::endl;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[2]);
            vertexIndices.push_back(vertexIndex[3]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[2]);
            uvIndices    .push_back(uvIndex[3]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[2]);
            normalIndices.push_back(normalIndex[3]);
        }
        
    }
    // For each vertex of each triangle
    for( unsigned int i=0; i<vertexIndices.size(); i++ ){
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
        verticesCoord.push_back(vertex);
    }
    // For each vertex of each texture
    for( unsigned int i=0; i<uvIndices.size(); i++ ){
        unsigned int uvIndex = uvIndices[i];
        glm::vec2 uv = temp_uvs[ uvIndex-1 ];
        textureCoords.push_back(uv);
    }
    // For each vertex of each normals
    for( unsigned int i=0; i<normalIndices.size(); i++ ){
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 normal = temp_normals[ normalIndex-1 ];
        verticesNormals.push_back(normal);
    }
    
    //std::cout << "Vertices : " << verticesCoord.size() << std::endl;
};

ModelOBJ::ModelOBJ(std::vector<glm::vec3> new_vertices, ModelOBJ o, std::string const vertexShader, std::string const fragmentShader, std::string const texture)  : m_shader(vertexShader, fragmentShader) {
    m_texture = texture;
    m_shader.charger();
    
    verticesCoord=new_vertices;
    verticesNormals=o.verticesNormals;
    textureCoords=o.textureCoords;
    vertexIndices=o.vertexIndices;
    normalIndices=o.normalIndices;
    textureIndices=o.textureIndices;
}

ModelOBJ::~ModelOBJ(){
    {
        // Destruction du VBO
        
        glDeleteBuffers(1, &m_vboID);
        
        
        // Destruction du VAO
        
        glDeleteVertexArrays(1, &m_vaoID);
    }
}

void ModelOBJ::charger()
{
    // Destruction d'un Èventuel ancien VBO
    
    if(glIsBuffer(m_vboID) == GL_TRUE)
        glDeleteBuffers(1, &m_vboID);
    
    
    // GÈnÈration de l'ID
    
    glGenBuffers(1, &m_vboID);
    
    
    // Verrouillage du VBO
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    
    
    // Allocation de la mÈmoire vidÈo
    
    glBufferData(GL_ARRAY_BUFFER,  (verticesCoord.size() * sizeof(glm::vec3)) + (textureCoords.size() * sizeof(glm::vec2)), 0, GL_STATIC_DRAW);
    
    
    // Transfert des donnÈes
    
    glBufferSubData(GL_ARRAY_BUFFER, 0, verticesCoord.size() * sizeof(glm::vec3), &verticesCoord[0]);
    glBufferSubData(GL_ARRAY_BUFFER, verticesCoord.size() * sizeof(glm::vec3), textureCoords.size() * sizeof(glm::vec2), &textureCoords[0]);
    
    
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
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(verticesCoord.size() * sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);
    
    
    // DÈverrouillage du VBO
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    // DÈverrouillage du VAO
    
    glBindVertexArray(0);
}


void ModelOBJ::afficher(glm::mat4 &projection, glm::mat4 &modelview)
{
    // Activation du shader
    
    glUseProgram(m_shader.getProgramID());
    
    
    // Verrouillage du VAO
    
    glBindVertexArray(m_vaoID);
    
    
    // Envoi des matrices
    
    glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
    
    // Verrouillage de la texture
    
    glBindTexture(GL_TEXTURE_2D, m_texture.getID());
    
    
    // Rendu
    
    glDrawArrays(GL_TRIANGLES, 0, (int) verticesCoord.size());
    
    
    // DÈverrouillage du VAO
    
    glBindVertexArray(0);
    
    
    // DÈsactivation du shader
    
    
    glUseProgram(0);
}

void ModelOBJ::updateVBO(void *donnees, int tailleBytes, int decalage)
{
    // Verrouillage du VBO
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    
    
    // RÈcupÈration de l'adresse du VBO
    
    void *adresseVBO = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    
    
    // Si l'adresse retournÈe est nulle alors on arrÍte le transfert
    
    if(adresseVBO == NULL)
    {
        std::cout << "Erreur au niveau de la rÈcupÈration du VBO" << std::endl;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        return;
    }
    
    
    // Mise ‡ jour des donnÈes
    
    memcpy((char*)adresseVBO + decalage, donnees, tailleBytes);
    
    
    // Annulation du pointeur
    
    glUnmapBuffer(GL_ARRAY_BUFFER);
    adresseVBO = 0;
    
    
    // DÈverrouillage du VBO
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

std::vector<glm::vec3> ModelOBJ::getVertices(){
    return verticesCoord;
}

void* ModelOBJ::getVerticesAddr(){
    return &verticesCoord;
}
