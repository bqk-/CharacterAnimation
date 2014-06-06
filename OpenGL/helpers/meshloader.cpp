#include "meshloader.h"

MeshLoader* MeshLoader::m_instance = NULL;

MeshLoader::MeshLoader()
{
}

MeshLoader& MeshLoader::get()
{
    if(m_instance == NULL)
        m_instance = new MeshLoader();

    return *m_instance;
}

Model3D* MeshLoader::openModel(std::string filename, ModelType ext)
{
    Model3D* res = NULL;

    // Open file according to the extension choose
    switch(ext)
    {
        case OBJ:
            res = createOBJ(filename);
            break;

        case DAE:
            res = createDAE(filename);
            break;

        default:
            res = createModel(filename);
            break;
    }

    return res;
}

Model3D* MeshLoader::createOBJ(std::string filename)
{
    // Open file
    QFile file(filename.c_str());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug("Cannot open file : %s",filename.c_str());
        return NULL;
    }

    // Init values
    std::vector<float> vert;
    std::vector<float> text;
    std::vector<float> norm;
    std::vector<int> indi;
    int nModel = 0;

    // Read file as stream
    QTextStream stream(&file);
    QString line = stream.readLine();

    // Read each line
    while (!line.isNull() && nModel<=1)
    {
        line.replace("  "," ");
        QStringList list = line.split(' ');
        QString element = list.at(0);

        // Create a group of geometry
        if(element == "g")
        {
            nModel++;
        }
        // Add vertices X,Y,Z values
        else if(element == "v")
        {
            vert.push_back(list.at(1).toFloat());
            vert.push_back(list.at(2).toFloat());
            vert.push_back(list.at(3).toFloat());
        }
        // Add coordinates texture values
        else if(element == "vt")
        {
            text.push_back(list.at(1).toFloat());
            text.push_back(list.at(2).toFloat());
            //qDebug("vt %f %f", list.at(1).toFloat(), list.at(2).toFloat());
        }
        // Add normals X,Y,Z
        else if(element == "vn")
        {
            norm.push_back(list.at(1).toFloat());
            norm.push_back(list.at(2).toFloat());
            norm.push_back(list.at(3).toFloat());
            //qDebug("vn %f %f %f", list.at(1).toFloat(), list.at(2).toFloat(), list.at(3).toFloat());
        }
        // Create faces
        else if(element == "f")
        {
            //std::string str = "";

            int size = list.size();

            for(int i=1; i<size; i++)
            {
                if(i < 4) {
                    QStringList listFace = list.at(i).split('/');
                    indi.push_back(listFace.at(0).toInt()-1);
                }
                else if(false)
                {
                    QStringList listFace = list.at(i).split('/');
                    QStringList listFace1 = list.at(i-1).split('/');
                    QStringList listFace2 = list.at(i-3).split('/');

                    indi.push_back(listFace.at(0).toInt()-1);
                    indi.push_back(listFace1.at(0).toInt()-1);
                    indi.push_back(listFace2.at(0).toInt()-1);
                }

                //str.append(" ");
                //str.append(listFace.at(0).toStdString().c_str());
                //str.append(" ");
            }
            //qDebug("f %s", str.c_str());
        }

        line = stream.readLine();
    }

    qDebug("vertices: %i \nindices: %i \ntexture: %i \nnormals: %i",vert.size()/3,indi.size(),text.size()/2,norm.size()/3);

    //Affect values
    Geometry* geo = new Geometry();
    geo->loadVertices(vert.data(), vert.size()/3);
    geo->loadNormals(norm.data(), norm.size()/3);
    geo->loadIndices(indi.data(), indi.size());
    geo->setPrimitive(GL_TRIANGLES);
    Material* mate = new Material();
    std::vector<float> color; float s = vert.size(); for(int i=0; i<s; i++) color.push_back(i/s);
    mate->loadColors(color.data(), color.size()/3);
    mate->loadTextureCoordinate(text.data(), text.size()/2);
    Mesh* mesh = new Mesh(geo,mate);

    return new Model3D(mesh);
}

Model3D* MeshLoader::createDAE(std::string filename)
{
    Model3D* res = new Model3D();

    // Init values Geometries
    std::vector<float> vert;
    std::vector<float> text;
    std::vector<float> norm;
    std::vector<int> indi;
    int nModel = 0;

    // Init values Controllers
    std::vector<float> ctrlBindShapeMatrix;
    std::vector<Joint*> ctrlJointList;
    std::vector<float> ctrlBindPose;
    std::vector<float> ctrlWeight;
    std::vector<int> ctrlWeightVCount;
    std::vector<int> ctrlWeightV;

    // Init values Animations
    std::vector<float> keytime;
    std::vector<float> mattime;
    std::vector<float> interpolation;
    std::vector<Keyframe*> keyframeList;
    Skeleton* skeleton = NULL;

    // Open file
    std::ifstream file(filename.c_str());
    if(file.fail()) {
        file.close();
        std::cout << "failed to open file: " << filename << std::endl;
        delete res;
        return NULL;
    } //Failed to open

    // Get content
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content(buffer.str());

    // Close file
    file.close();

    // Create xml document
    xml_document<> doc; // character type defaults to char
    doc.parse<0>(&content[0]);

    // Read ROOT NODE
    xml_node<> *root = doc.first_node(doc.first_node()->name());

    // Read ROOT ATTRIBUTES
    for (xml_attribute<> *attr = root->first_attribute(); attr; attr = attr->next_attribute())
    {
    }

    // Read NODES
    for(xml_node<> *node=root->first_node("asset"); node; node=node->next_sibling())
    {
        // LIBRARY_GEOMETRIES
        if(!strcmp(node->name(),"library_geometries"))
        {
            // GEOMETRY
            for(xml_node<> *nodeGeometry=node->first_node("geometry"); nodeGeometry; nodeGeometry=nodeGeometry->next_sibling())
            {
                // Attributes
                for (xml_attribute<> *attr = nodeGeometry->first_attribute(); attr; attr = attr->next_attribute())
                {
                    char* str = attr->value();

                    // Get content (id)
                    if(!strcmp(attr->name(),"id"))
                    {

                    }

                    // Get content (name)
                    if(!strcmp(attr->name(),"name"))
                    {

                    }
                }

                // MESH
                xml_node<> *nodeMesh=nodeGeometry->first_node("mesh");

                // SOURCE | VERTICES | TRIANGLES
                int index=0;
                for(xml_node<> *nodeSource=nodeMesh->first_node("source"); nodeSource; nodeSource=nodeSource->next_sibling())
                {
                    // SOURCE
                    if(!strcmp(nodeSource->name(),"source"))
                    {
                        // Attributes
                        for (xml_attribute<> *attr = nodeSource->first_attribute(); attr; attr = attr->next_attribute())
                        {
                            char* str = attr->value();

                            // Get content (id)
                            if(!strcmp(attr->name(),"id"))
                            {

                            }

                            // Get content (name)
                            if(!strcmp(attr->name(),"name"))
                            {

                            }
                        }

                        // FLOAT_ARRAY | TECHNIQUE_COMMON
                        for(xml_node<> *nodeFloatArray=nodeSource->first_node("float_array"); nodeFloatArray; nodeFloatArray=nodeFloatArray->next_sibling())
                        {
                            char* str = nodeFloatArray->value();

                            // Get content (float_array)
                            if(!strcmp(nodeFloatArray->name(),"float_array"))
                            {
                                std::string id = "";
                                int count = 0;

                                // Attributes
                                for (xml_attribute<> *attr = nodeFloatArray->first_attribute(); attr; attr = attr->next_attribute())
                                {
                                    char* str = attr->value();

                                    // Get content (id)
                                    if(!strcmp(attr->name(),"id"))
                                    {
                                        id = str;
                                    }

                                    // Get content (count)
                                    if(!strcmp(attr->name(),"count"))
                                    {
                                        count = QString(str).toInt();
                                    }
                                }

                                // Values
                                QString strGet = str;                   // Cast to QString
                                strGet.replace(QRegExp("[\\n]")," ");   // Replace \n by space
                                QStringList strList;                    // Init a QStringList
                                strList = strGet.split(" ");            // Create the list from string separate with space
                                strList.removeAll("");                  // Remove empty string

                                // Create a list from data
                                for(int i=0; i<strList.size(); i++)
                                {
                                    if(index == 0)                                  // Vertices
                                        vert.push_back(strList.at(i).toFloat());
                                    else if(index == 1)                             // Normals
                                        norm.push_back(strList.at(i).toFloat());
                                    else if(index == 2)                             // Coordinates texture
                                        text.push_back(strList.at(i).toFloat());
                                }
                            }

                            // Get content (technique_common)
                            if(!strcmp(nodeFloatArray->name(),"technique_common"))
                            {

                            }
                        }
                    }

                    // VERTICES
                    if(!strcmp(nodeSource->name(),"vertices"))
                    {
                        // Attributes
                        for (xml_attribute<> *attr = nodeSource->first_attribute(); attr; attr = attr->next_attribute())
                        {
                            char* str = attr->value();

                            // Get content (id)
                            if(!strcmp(attr->name(),"id"))
                            {

                            }
                        }

                        // Input
                        for(xml_node<> *nodeVertices=nodeSource->first_node("input");nodeVertices; nodeVertices=nodeVertices->next_sibling())
                        {
                            char* str = nodeVertices->value();

                            // Get content (input)
                            if(!strcmp(nodeVertices->name(),"input"))
                            {
                                // Attributes
                                for (xml_attribute<> *attr = nodeVertices->first_attribute(); attr; attr = attr->next_attribute())
                                {
                                    char* str = attr->value();

                                    // Get content (semantic)
                                    if(!strcmp(attr->name(),"semantic"))
                                    {

                                    }

                                    // Get content (source)
                                    if(!strcmp(attr->name(),"source"))
                                    {

                                    }
                                }
                            }
                        }
                    }

                    // TRIANGLES
                    if(!strcmp(nodeSource->name(),"triangles"))
                    {
                        int nbInput = 0;
                        int count = 0;

                        // Attributes
                        for (xml_attribute<> *attr = nodeSource->first_attribute(); attr; attr = attr->next_attribute())
                        {
                            char* str = attr->value();

                            // Get content (material)
                            if(!strcmp(attr->name(),"material"))
                            {

                            }

                            // Get content (count)
                            if(!strcmp(attr->name(),"count"))
                            {
                                count = QString(str).toInt();
                            }
                        }

                        // Input | p
                        for(xml_node<> *nodeTriangles=nodeSource->first_node("input"); nodeTriangles; nodeTriangles=nodeTriangles->next_sibling())
                        {
                            char* str = nodeTriangles->value();

                            // Get content (input)
                            if(!strcmp(nodeTriangles->name(),"input"))
                            {
                                nbInput++;
                            }

                            // Get content (p)
                            if(!strcmp(nodeTriangles->name(),"p"))
                            {
                                QString strGet = str;
                                QStringList strList = strGet.split(" ");
                                for(int i=0; i<strList.size(); i++)
                                {
                                    // load vertices indices to list
                                    if(i%nbInput == 0)                          //Indices (Vertex)
                                    {
                                        indi.push_back(strList.at(i).toInt());
                                    }
                                    else if(i%nbInput == 1)                     //Indices (Normals)
                                    {

                                    }
                                    else if(i%nbInput == 2)                     //Indices (Textures)
                                    {

                                    }
                                }
                            }
                        }
                    }

                    // POLYGONS
                    if(!strcmp(nodeSource->name(),"polygons") || !strcmp(nodeSource->name(),"polylist"))
                    {
                        int nbInput = 0;
                        int count = 0;

                        // Attributes
                        for (xml_attribute<> *attr = nodeSource->first_attribute(); attr; attr = attr->next_attribute())
                        {
                            char* str = attr->value();

                            // Get content (material)
                            if(!strcmp(attr->name(),"material"))
                            {

                            }

                            // Get content (count)
                            if(!strcmp(attr->name(),"count"))
                            {
                                count = QString(str).toInt();
                            }
                        }

                        // Input | p
                        for(xml_node<> *nodePolygons=nodeSource->first_node("input"); nodePolygons; nodePolygons=nodePolygons->next_sibling())
                        {
                            char* str = nodePolygons->value();

                            // Get content (input)
                            if(!strcmp(nodePolygons->name(),"input"))
                            {
                                nbInput++;
                            }

                            // Get content (p) (Indices)
                            if(!strcmp(nodePolygons->name(),"p"))
                            {
                                QString strGet = str;
                                QStringList strList = strGet.split(" ");
                                for(int i=0; i<strList.size(); i++)
                                {
                                    // load vertices indices to list
                                    if(i%nbInput == 0)                          //Indices (Vertex)
                                    {
                                        indi.push_back(strList.at(i).toInt());
                                    }
                                    else if(i%nbInput == 1)                     //Indices (Normals)
                                    {

                                    }
                                    else if(i%nbInput == 2)                     //Indices (Textures)
                                    {

                                    }
                                }
                            }
                        }
                    }

                    // Next index
                    index++;
                }

                // Create 3D Mesh
                //Affect values
                Geometry* geo = new Geometry();
                geo->loadVertices(vert.data(), vert.size()/3);
                geo->loadNormals(norm.data(), norm.size()/3);
                geo->loadIndices(indi.data(), indi.size());
                geo->setPrimitive(GL_TRIANGLES);

                Material* mate = new Material();
                std::vector<float> color; float s = vert.size(); for(int i=0; i<s; i++) color.push_back(i/s); //i/s
                mate->loadColors(color.data(), color.size()/3);
                mate->loadTextureCoordinate(text.data(), text.size()/2);

                Mesh* mesh = new Mesh(geo,mate);
                res->addMesh(mesh);

                // Reset list of data
                vert.clear();
                norm.clear();
                text.clear();
                indi.clear();

                // Next Model
                nModel++;
            }

            std::cout << "library geometry: \t OK" << std::endl;
        }

        // LIBRARY_CONTROLLERS
        if(!strcmp(node->name(),"library_controllers"))
        {
            // ANIMATION
            xml_node<> *nodeController=node->first_node("controller");

            // SKIN
            if(nodeController)
            {
                for(xml_node<> *nodeSkin=nodeController->first_node("skin"); nodeSkin; nodeSkin=nodeSkin->next_sibling())
                {
                    // BIND_SHAPE_MATRIX | SOURCE | JOINTS | VERTEX_WEIGHT
                    int index=0;
                    for(xml_node<> *nodeSource=nodeSkin->first_node("bind_shape_matrix"); nodeSource; nodeSource=nodeSource->next_sibling())
                    {
                        // BIND_SHAPE_MATRIX
                        if(!strcmp(nodeSource->name(),"bind_shape_matrix"))
                        {
                            char* str = nodeSource->value();

                            // Values
                            QString strGet = str;                   // Cast to QString
                            strGet.replace(QRegExp("[\\n]")," ");   // Replace \n by space
                            QStringList strList;                    // Init a QStringList
                            strList = strGet.split(" ");            // Create the list from string separate with space
                            strList.removeAll("");                  // Remove empty string

                            // Create a list from data
                            for(int i=0; i<strList.size(); i++)
                            {
                                ctrlBindShapeMatrix.push_back(strList.at(i).toFloat());
                            }
                        }

                        // SOURCE
                        if(!strcmp(nodeSource->name(),"source"))
                        {
                            // NAME_ARRAY | TECHNIQUE_COMMON
                            for(xml_node<> *nodeNameArray=nodeSource->first_node("Name_array"); nodeNameArray; nodeNameArray=nodeNameArray->next_sibling())
                            {
                                char* str = nodeNameArray->value();

                                // Get content (Name_array)
                                if(!strcmp(nodeNameArray->name(),"Name_array"))
                                {
                                    // Values
                                    QString strGet = str;                   // Cast to QString
                                    strGet.replace(QRegExp("[\\n]")," ");   // Replace \n by space
                                    QStringList strList;                    // Init a QStringList
                                    strList = strGet.split(" ");            // Create the list from string separate with space
                                    strList.removeAll("");                  // Remove empty string

                                    // Create a list from data
                                    for(int i=0; i<strList.size(); i++)
                                    {
                                        Joint* j = new Joint(i);
                                        j->setName(strList.at(i).toStdString());
                                        ctrlJointList.push_back(j);
                                    }
                                }

                                // Get content (technique_common)
                                if(!strcmp(nodeNameArray->name(),"technique_common"))
                                {

                                }
                            }

                            // FLOAT_ARRAY | TECHNIQUE_COMMON
                            for(xml_node<> *nodeFloatArray=nodeSource->first_node("float_array"); nodeFloatArray; nodeFloatArray=nodeFloatArray->next_sibling())
                            {
                                char* str = nodeFloatArray->value();

                                // Get content (float_array)
                                if(!strcmp(nodeFloatArray->name(),"float_array"))
                                {
                                    std::string id = "";
                                    int count = 0;

                                    // Attributes
                                    for (xml_attribute<> *attr = nodeFloatArray->first_attribute(); attr; attr = attr->next_attribute())
                                    {
                                        char* str = attr->value();

                                        // Get content (id)
                                        if(!strcmp(attr->name(),"id"))
                                        {
                                            id = str;
                                        }

                                        // Get content (count)
                                        if(!strcmp(attr->name(),"count"))
                                        {
                                            count = QString(str).toInt();
                                        }
                                    }

                                    // Values
                                    QString strGet = str;                   // Cast to QString
                                    strGet.replace(QRegExp("[\\n]")," ");   // Replace \n by space
                                    QStringList strList;                    // Init a QStringList
                                    strList = strGet.split(" ");            // Create the list from string separate with space
                                    strList.removeAll("");                  // Remove empty string

                                    // Create a list from data
                                    for(int i=0; i<strList.size(); i++)
                                    {
                                        if(index == 2)                             // Bind Pose Matrix
                                            ctrlBindPose.push_back(strList.at(i).toFloat());
                                        else if(index == 3)                        // Weight
                                        {
                                            res->getMesh()->getGeometry()->addVertexWeight(strList.at(i).toFloat());
                                            ctrlWeight.push_back(strList.at(i).toFloat());
                                        }
                                    }
                                }

                                // Get content (technique_common)
                                if(!strcmp(nodeFloatArray->name(),"technique_common"))
                                {

                                }
                            }
                        }

                        // JOINTS
                        if(!strcmp(nodeSource->name(),"joints"))
                        {
                            // INPUT
                            for(xml_node<> *nodeInput=nodeSource->first_node("input"); nodeInput; nodeInput=nodeInput->next_sibling())
                            {
                                char* str = nodeInput->value();

                                // Get content (input)
                                if(!strcmp(nodeInput->name(),"input"))
                                {
                                }
                            }
                        }

                        // VERTEX_WEIGHT
                        if(!strcmp(nodeSource->name(),"vertex_weights"))
                        {
                            int nbInput = 0;
                            int count = 0;

                            // Attributes
                            for (xml_attribute<> *attr = nodeSource->first_attribute(); attr; attr = attr->next_attribute())
                            {
                                char* str = attr->value();

                                // Get content (count)
                                if(!strcmp(attr->name(),"count"))
                                {
                                    count = QString(str).toInt();
                                }
                            }

                            // Input | vcount | v
                            for(xml_node<> *nodeWeight=nodeSource->first_node("input"); nodeWeight; nodeWeight=nodeWeight->next_sibling())
                            {
                                char* str = nodeWeight->value();

                                // Get content (input)
                                if(!strcmp(nodeWeight->name(),"input"))
                                {
                                    nbInput++;
                                }

                                // Get content (vcount)
                                if(!strcmp(nodeWeight->name(),"vcount"))
                                {
                                    QString strGet = str;
                                    QStringList strList = strGet.split(" ");
                                    for(int i=0; i<strList.size(); i++)
                                    {
                                        ctrlWeightVCount.push_back(strList.at(i).toInt());
                                    }
                                }

                                // Get content (v)
                                if(!strcmp(nodeWeight->name(),"v"))
                                {
                                    QString strGet = str;
                                    QStringList strList = strGet.split(" ");
                                    for(int i=0; i<strList.size(); i++)
                                    {
                                        ctrlWeightV.push_back(strList.at(i).toInt());
                                    }
                                }
                            }
                        }

                        // Next index
                        index++;
                    }

                    // Give Matrix4x4 Values to joint
                    int nbJoint = ctrlJointList.size();
                    if(ctrlBindPose.size() != 0)
                    {
                        for(int iJoint=0; iJoint<nbJoint; iJoint++)
                        {
                            Joint *joint = ctrlJointList.at(iJoint);
                            int idJoint = joint->getId();
                            GLMatrix matrice;
                            int matSize = 16;
                            for(int j=0; j<matSize; j++)
                                matrice.data[j] = ctrlBindPose.at((idJoint*matSize)+j);
                            joint->setBindPoseMatrix(matrice);
                        }
                    }

                    // CREATE THE LIST OF VertexInfluence USED FOR BONES
                    int sizeVCount = ctrlWeightVCount.size();
                    int vcount = 0;
                    for(int iId=0; iId<sizeVCount; iId++)
                    {
                        int nbInfluence = ctrlWeightVCount.at(iId);
                        VertexInfluence* v = new VertexInfluence();
                        for(int iInfluence=0; iInfluence<nbInfluence; iInfluence++)
                        {
                            // Get id's
                            int idJoint = ctrlWeightV.at(vcount*2+0);
                            int idWeight = ctrlWeightV.at(vcount*2+1);

                            v->addJoint(idJoint);
                            v->addWeight(idWeight);

                            vcount++;
                        }

                        // Add VertexInfluence to list
                        res->getMesh()->getGeometry()->addVertexInfluence(v);
                    }
                }
            }

            std::cout << "library controller: \t OK" << std::endl;
        }

        // LIBRARY_ANIMATIONS
        if(!strcmp(node->name(),"library_animations"))
        {
            // ANIMATION
            xml_node<> *nodeAnimation=node->first_node("animation");

            // SOURCE | SAMPLER | CHANNEL
            int index=0;
            for(xml_node<> *nodeSource=nodeAnimation->first_node("source"); nodeSource; nodeSource=nodeSource->next_sibling())
            {
                // SOURCE
                if(!strcmp(nodeSource->name(),"source"))
                {
                    // FLOAT_ARRAY | TECHNIQUE_COMMON
                    for(xml_node<> *nodeFloatArray=nodeSource->first_node("float_array"); nodeFloatArray; nodeFloatArray=nodeFloatArray->next_sibling())
                    {
                        char* str = nodeFloatArray->value();

                        // Get content (float_array)
                        if(!strcmp(nodeFloatArray->name(),"float_array"))
                        {
                            std::string id = "";
                            int count = 0;

                            // Attributes
                            for (xml_attribute<> *attr = nodeFloatArray->first_attribute(); attr; attr = attr->next_attribute())
                            {
                                char* str = attr->value();

                                // Get content (id)
                                if(!strcmp(attr->name(),"id"))
                                {
                                    id = str;
                                }

                                // Get content (count)
                                if(!strcmp(attr->name(),"count"))
                                {
                                    count = QString(str).toInt();
                                }
                            }

                            // Values
                            QString strGet = str;                   // Cast to QString
                            strGet.replace(QRegExp("[\\n]")," ");   // Replace \n by space
                            QStringList strList;                    // Init a QStringList
                            strList = strGet.split(" ");            // Create the list from string separate with space
                            strList.removeAll("");                  // Remove empty string

                            // Create a list from data
                            for(int i=0; i<strList.size(); i++)
                            {
                                if(index == 0)                                  // Keytime
                                    keytime.push_back(strList.at(i).toFloat());
                                else if(index == 1)                             // Mattime
                                    mattime.push_back(strList.at(i).toFloat());
                                else if(index == 2)                             // Interpolation
                                    interpolation.push_back(strList.at(i).toFloat());
                            }
                        }

                        // Get content (technique_common)
                        if(!strcmp(nodeFloatArray->name(),"technique_common"))
                        {

                        }
                    }
                }

                // SAMPLER
                if(!strcmp(nodeSource->name(),"sampler"))
                {

                }

                // CHANNEL
                if(!strcmp(nodeSource->name(),"channel"))
                {

                }
            }

            std::cout << "library animation: \t OK" << std::endl;
        }

        // LIBRARY_VISUAL_SCENES
        if(!strcmp(node->name(),"library_visual_scenes"))
        {
            // VISUAL SCENE
            xml_node<> *nodeVisualScene=node->first_node("visual_scene");

            for(xml_node<> *nodeNode=nodeVisualScene->first_node("node"); nodeNode; nodeNode=nodeNode->next_sibling())
            {
                // Create root joint skeleton
                Joint* root = new Joint();
                std::string type = "";

                // Get skeleton name
                for (xml_attribute<> *attr = nodeNode->first_attribute(); attr; attr = attr->next_attribute())
                {
                    char* str = attr->value();

                    // Get content (id)
                    if(!strcmp(attr->name(),"id"))
                    {
                    }

                    // Get content (name)
                    if(!strcmp(attr->name(),"name"))
                    {
                        std::string name = str;
                        root->setName(name);
                    }

                    // Get content (type)
                    if(!strcmp(attr->name(),"type"))
                    {
                        type = str;
                    }
                }

                // Skeleton
                if(type == "JOINT")
                {
                    // Skeleton construction
                    createSkeleton(root, nodeNode, ctrlJointList);
                    skeleton = new Skeleton(root);
                }
                // NODE
                else if(type == "NODE")
                {
                    delete root;
                }
                // OTHER
                else
                {
                    delete root;
                }
            }
            std::cout << "library visual: \t OK" << std::endl;
        }
    }

    // DEBUG
    if(skeleton) drawSkeletonStructure(skeleton->getRoot());
    // DEBUG

    if(skeleton)
    {
        // Affect skeleton
        res->setSkeleton(skeleton);

        // Fill joint list
        for(int i=0; i<ctrlJointList.size(); i++)
            skeleton->addJoint(ctrlJointList.at(i));

        // Set Bind Shape Matrix to Skeleton
        GLMatrix mat;
        for(int i=0; i<ctrlBindShapeMatrix.size(); i++)
            mat.data[i] = ctrlBindShapeMatrix.at(i);
        skeleton->setBindShapeMatrix(mat);

        // Setup skeleton
        res->setupSkeleton();

        // Bind pose
        res->setupBindPose();

        // Update mesh
        res->getMesh()->updateSkinned();
    }

    // Create the 3D Model
    return res;
}

void MeshLoader::drawSkeletonStructure(Joint *j, int i)
{
    int cpt = i;
    if(j)
    {
        for(int i=0;i<cpt;i++) std::cout << "-";
        std::cout << j->getName() << "(" << j->getId() << ")";

        if(j->getId() != -1)
        {
            //std::cout << " bind pose matrix: " << j->getBindPoseMatrix().data[0] << "|" << j->getBindPoseMatrix().data[1];
        }

        std::cout << std::endl;

        for(int i=0; i<j->getNbChild(); i++)
        {
            drawSkeletonStructure(j->getChild(i),cpt+1);
        }
    }
}

Model3D* MeshLoader::createModel(std::string filename)
{
    if(filename.find(".obj")!=std::string::npos)
        return createOBJ(filename);
    else if(filename.find(".dae")!=std::string::npos)
        return createDAE(filename);
    else
        return NULL;
}

void MeshLoader::createSkeleton(Joint *&joint, xml_node<> *node, std::vector<Joint *> &ctrlJointList)
{
    for(xml_node<> *nodeNode=node->first_node("matrix"); nodeNode; nodeNode=nodeNode->next_sibling())
    {
        char* str = nodeNode->value();

        // MATRIX
        if(!strcmp(nodeNode->name(),"matrix"))
        {
            // Get matrix4x4 values
            QString strGet = str;                   // Cast to QString
            strGet.replace(QRegExp("[\\n]")," ");   // Replace \n by space
            QStringList strList;                    // Init a QStringList
            strList = strGet.split(" ");            // Create the list from string separate with space
            strList.removeAll("");                  // Remove empty string

            // Set Transform Matrix to joint
            GLMatrix mat;
            for(int i=0; i<strList.size(); i++)
                mat.data[i] = strList.at(i).toFloat();
            joint->setTransformMatrix(mat);
        }

        // NODE
        if(!strcmp(nodeNode->name(),"node"))
        {
            // Init
            std::string name = "";
            std::string sid = "";

            // Read attributes
            for (xml_attribute<> *attr = nodeNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                char* str = attr->value();

                // Get content (id)
                if(!strcmp(attr->name(),"id"))
                {
                }

                // Get content (name)
                if(!strcmp(attr->name(),"name"))
                {
                    name = str;
                }

                // Get content (sid)
                if(!strcmp(attr->name(),"sid"))
                {
                    sid = str;
                }
            }

            // Get Joint & Id in ctrlJointList from name
            int idJoint = -1;
            Joint* j = NULL;
            for(int i=0; i<ctrlJointList.size(); i++)
            {
                j = ctrlJointList.at(i);
                std::string nname = j->getName();
                if(nname == name || nname == sid)
                {
                    idJoint = i;            // Get joint Id
                    i = ctrlJointList.size();   // End loop
                }
            }

            // Relaunch creation on the child joint
            if(idJoint != -1 && j)
            {
                // Add child joint
                joint->addChild(j);

                // Get childs of the child
                createSkeleton(j, nodeNode, ctrlJointList);
            }
            // If joint doesn't exists
            else
            {
                Joint* nJoint = new Joint();

                // Add child joint
                joint->addChild(nJoint);
                if(sid != "")
                    nJoint->setName(sid);
                else
                    nJoint->setName(name);

                // Get childs of the child
                createSkeleton(nJoint, nodeNode, ctrlJointList);
            }
        }
    }
}

Model3D* MeshLoader::createAxis(float size)
{
    float tabVertices[]=
    {
        0,      0,      0,
        size,   0,      0,
        0,      0,      0,
        0,      size,   0,
        0,      0,      0,
        0,      0,      size
    };

    float tabColors[]=
    {
        1,0,0,
        1,0,0,
        0,1,0,
        0,1,0,
        0,0,1,
        0,0,1
    };

    Geometry* g1 = new Geometry();
    g1->loadVertices(tabVertices,6);
    g1->setPrimitive(GL_LINES);
    Material* m1 = new Material();
    m1->loadColors(tabColors,6);
    Mesh* mesh1 = new Mesh(g1,m1);
    return new Model3D(mesh1);
}

/*
    //Affect values
    Geometry* geo = new Geometry();
    qDebug("Geometry created : \tOK");
    geo->loadVertices(vert.data(), vert.size()/3);
    qDebug("Geometry vertices : \tOK (%i)",vert.size());
    geo->loadNormals(norm.data(), norm.size()/3);
    qDebug("Geometry normals : \tOK (%i)",norm.size());
    geo->loadIndices(indi.data(), indi.size());
    qDebug("Geometry indices : \tOK (%i)",indi.size());
    geo->setPrimitive(GL_TRIANGLES);
    qDebug("-Geometry build-");

    Material* mate = new Material();
    qDebug("Material created : \tOK");

    //default colors
    std::vector<float> color;
    float s = vert.size();
    for(int i=0; i<s; i++)
        color.push_back(i/s);

    mate->loadColors(color.data(), color.size()/3);
    qDebug("Material Color load : \tOK (%i)",color.size());
    mate->loadTextureCoordinate(text.data(), text.size()/2);
    qDebug("Material Text load : \tOK (%i)",text.size());
    qDebug("-Material build-");

    qDebug("-Mesh build-");
  */
