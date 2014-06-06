#include "mesh.h"

Mesh::Mesh(Geometry *geometry, Material *material) : m_geometry(geometry), m_material(material)
{
    initVBO();
}

Mesh::~Mesh()
{
    glDeleteBuffers(0, &m_bufVertices);
    glDeleteBuffers(0, &m_bufColors);
    glDeleteBuffers(0, &m_bufIndices);
    glDeleteBuffers(0, &m_bufNormals);
    //glDeleteBuffers(0, &m_bufTex);
    delete m_geometry;
    delete m_material;
}

void Mesh::initVBO()
{
    // Vertices
    int nbVertices = m_geometry->getNbVertices();
    if(m_geometry->hasVertices())
    {
        glGenBuffers(1, &m_bufVertices);
        glBindBuffer(GL_ARRAY_BUFFER, m_bufVertices);
        glBufferData(GL_ARRAY_BUFFER, nbVertices*3*sizeof(float), 0, GL_STATIC_DRAW);
        float* dataBufVertices = (float*)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        float* dataVertices = m_geometry->getVertices();
        for(int i=0; i<nbVertices*3; i++)
            dataBufVertices[i] = dataVertices[i];
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    // Colors
    int nbColors = m_material->getNbColors();
    if(m_material->hasColors())
    {
        glGenBuffers(1, &m_bufColors);
        glBindBuffer(GL_ARRAY_BUFFER, m_bufColors);
        glBufferData(GL_ARRAY_BUFFER, nbColors*3*sizeof(float), 0, GL_DYNAMIC_DRAW);
        float* dataBufColors = (float*)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        float* dataColors = m_material->getColors();
        for(int i=0; i<nbColors*3; i++)
            dataBufColors[i] = dataColors[i];
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    // Normals
    int nbNormals = m_geometry->getNbNormals();
    if(m_geometry->hasNormals())
    {
        glGenBuffers(1, &m_bufNormals);
        glBindBuffer(GL_ARRAY_BUFFER, m_bufNormals);
        glBufferData(GL_ARRAY_BUFFER, nbNormals*3*sizeof(float), 0, GL_STATIC_DRAW);
        float* dataBufNormals = (float*)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        float* dataNormals = m_geometry->getNormals();
        for(int i=0; i<nbNormals; i++)
            dataBufNormals[i] = dataNormals[i];
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    /*/ Tex coordinates
    int nbTexCoord = m_material->getNbUvs();
    if(m_material->hasTextureCoordinates())
    {
        glGenBuffers(1, &m_bufTex);
        glBindBuffer(GL_ARRAY_BUFFER, m_bufTex);
        glBufferData(GL_ARRAY_BUFFER, nbTexCoord*2*sizeof(float), 0, GL_STATIC_DRAW);
        float* dataBufTex = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        float* dataTex = m_material->getUvs();
        for(int i=0; i<nbTexCoord*2; i++)
           dataBufTex[i] = dataTex[i];
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    */

    // Indices
    int nbIndices = m_geometry->getNbIndices();
    if(m_geometry->hasIndices())
    {
        glGenBuffers(1, &m_bufIndices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufIndices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbIndices*sizeof(int), 0, GL_STATIC_DRAW);
        int* dataBufIndices = (int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,GL_WRITE_ONLY);
        int* dataIndices = m_geometry->getIndices();
        for(int i=0; i<nbIndices; i++)
            dataBufIndices[i] = dataIndices[i];
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    }

    // Un-bind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::drawShape()
{
    // Vertices
    GLint var1 = glGetAttribLocation( m_Framework->getCurrentShaderId(), "position" );
    glEnableVertexAttribArray( var1 );
    glBindBuffer(GL_ARRAY_BUFFER, m_bufVertices);
    glVertexAttribPointer( var1, 3, GL_FLOAT, GL_FALSE, 0, 0 );

    // Colors
    GLint var2 = glGetAttribLocation( m_Framework->getCurrentShaderId(), "color" );
    glEnableVertexAttribArray( var2 );
    glBindBuffer(GL_ARRAY_BUFFER, m_bufColors);
    glVertexAttribPointer( var2, 3, GL_FLOAT, GL_FALSE, 0, 0 );

    /*/ Texture coordinate
    GLint var3 = glGetAttribLocation( m_Framework->getCurrentShaderId(), "texcoord" );
    if(material->getNbUvs() != 0)
    glEnableVertexAttribArray( var3 );
    glBindBuffer(GL_ARRAY_BUFFER, m_bufTex);
    glVertexAttribPointer( var3, 2, GL_FLOAT, GL_FALSE, 0, 0);
    */

    // Normals
    GLint var4 = glGetAttribLocation( m_Framework->getCurrentShaderId(), "normal" );
    if(m_geometry->getNbNormals() != 0)
    {
        glEnableVertexAttribArray( var4 );
        glBindBuffer(GL_ARRAY_BUFFER, m_bufNormals);
        glVertexAttribPointer( var4, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // Indices
    if(m_geometry->getNbIndices() != 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufIndices);
        glDrawElements(m_geometry->getPrimitive(), m_geometry->getNbIndices(), GL_UNSIGNED_INT, 0);
    }
    else
        glDrawArrays( m_geometry->getPrimitive(), 0, m_geometry->getNbVertices());

    // Free Shader
    glDisableVertexAttribArray( var1 );
    glDisableVertexAttribArray( var2 );
    //glDisableVertexAttribArray( var3 );
    glDisableVertexAttribArray( var4 );
}

void Mesh::update()
{
    // Vertices Update
    int nbVertices = m_geometry->getNbVertices();
    if(m_geometry->hasVertices())
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufVertices);
        glBufferData(GL_ARRAY_BUFFER, nbVertices*3*sizeof(float), 0, GL_DYNAMIC_DRAW);
        float* dataBufVertices = (float*)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        float* dataVertices = m_geometry->getVertices();
        for(int i=0; i<nbVertices*3; i++)
            dataBufVertices[i] = dataVertices[i];
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    // Colors
    int nbColors = m_material->getNbColors();
    if(m_material->hasColors())
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufColors);
        glBufferData(GL_ARRAY_BUFFER, nbColors*3*sizeof(float), 0, GL_DYNAMIC_DRAW);
        float* dataBufColors = (float*)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        float* dataColors = m_material->getColors();
        for(int i=0; i<nbColors*3; i++)
            dataBufColors[i] = dataColors[i];
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    // Normals
    int nbNormals = m_geometry->getNbNormals();
    if(m_geometry->hasNormals())
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufNormals);
        glBufferData(GL_ARRAY_BUFFER, nbNormals*3*sizeof(float), 0, GL_DYNAMIC_DRAW);
        float* dataBufNormals = (float*)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        float* dataNormals = m_geometry->getNormals();
        for(int i=0; i<nbNormals; i++)
            dataBufNormals[i] = dataNormals[i];
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    /*/ Tex coordinates
    int nbTexCoord = m_material->getNbUvs();
    if(m_material->hasTextureCoordinates())
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufTex);
        glBufferData(GL_ARRAY_BUFFER, nbTexCoord*2*sizeof(float), 0, GL_DYNAMIC_DRAW);
        float* dataBufTex = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        float* dataTex = m_material->getUvs();
        for(int i=0; i<nbTexCoord*2; i++)
           dataBufTex[i] = dataTex[i];
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    */

    // Indices
    int nbIndices = m_geometry->getNbIndices();
    if(m_geometry->hasIndices())
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufIndices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbIndices*sizeof(int), 0, GL_DYNAMIC_DRAW);
        int* dataBufIndices = (int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,GL_WRITE_ONLY);
        int* dataIndices = m_geometry->getIndices();
        for(int i=0; i<nbIndices; i++)
            dataBufIndices[i] = dataIndices[i];
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    }

    // Un-bind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::updateSkinned()
{
    // Vertices Update
    int nbVertices = m_geometry->getNbVertices();
    if(m_geometry->hasVertices())
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufVertices);
        glBufferData(GL_ARRAY_BUFFER, nbVertices*3*sizeof(float), 0, GL_DYNAMIC_DRAW);
        float* dataBufVertices = (float*)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        float* dataVertices = m_geometry->getVerticesSkinned();
        for(int i=0; i<nbVertices*3; i++)
            dataBufVertices[i] = dataVertices[i];
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    // Colors
    int nbColors = m_material->getNbColors();
    if(m_material->hasColors())
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufColors);
        glBufferData(GL_ARRAY_BUFFER, nbColors*3*sizeof(float), 0, GL_DYNAMIC_DRAW);
        float* dataBufColors = (float*)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        float* dataColors = m_material->getColors();
        for(int i=0; i<nbColors*3; i++)
            dataBufColors[i] = dataColors[i];
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    // Normals
    int nbNormals = m_geometry->getNbNormals();
    if(m_geometry->hasNormals())
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufNormals);
        glBufferData(GL_ARRAY_BUFFER, nbNormals*3*sizeof(float), 0, GL_DYNAMIC_DRAW);
        float* dataBufNormals = (float*)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        float* dataNormals = m_geometry->getNormalsSkinned();
        for(int i=0; i<nbNormals; i++)
            dataBufNormals[i] = dataNormals[i];
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    /*/ Tex coordinates
    int nbTexCoord = m_material->getNbUvs();
    if(m_material->hasTextureCoordinates())
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufTex);
        glBufferData(GL_ARRAY_BUFFER, nbTexCoord*2*sizeof(float), 0, GL_DYNAMIC_DRAW);
        float* dataBufTex = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        float* dataTex = m_material->getUvs();
        for(int i=0; i<nbTexCoord*2; i++)
           dataBufTex[i] = dataTex[i];
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    */

    // Indices
    int nbIndices = m_geometry->getNbIndices();
    if(m_geometry->hasIndices())
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufIndices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbIndices*sizeof(int), 0, GL_DYNAMIC_DRAW);
        int* dataBufIndices = (int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,GL_WRITE_ONLY);
        int* dataIndices = m_geometry->getIndices();
        for(int i=0; i<nbIndices; i++)
            dataBufIndices[i] = dataIndices[i];
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    }

    // Un-bind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
