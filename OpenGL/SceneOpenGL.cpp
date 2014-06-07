#include "SceneOpenGL.h"


// Permet d'éviter la ré-écriture du namespace glm::

using namespace glm;


// Constructeur de Destucteur

SceneOpenGL::SceneOpenGL(std::string titreFenetre, int largeurFenetre, int hauteurFenetre) : m_titreFenetre(titreFenetre), m_largeurFenetre(largeurFenetre),
                                                                                             m_hauteurFenetre(hauteurFenetre), m_fenetre(0), m_contexteOpenGL(0),
                                                                                             m_input()
{

}


SceneOpenGL::~SceneOpenGL()
{
    SDL_GL_DeleteContext(m_contexteOpenGL);
    SDL_DestroyWindow(m_fenetre);
    SDL_Quit();
}


// Méthodes

bool SceneOpenGL::initialiserFenetre()
{
    // Initialisation de la SDL

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }


    #ifdef __APPLE__

        // Version d'OpenGL

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


        // Récupération du Bundle

        CFURLRef URLBundle = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
        char *cheminResources = new char[PATH_MAX];


        // Changement du 'Working Directory'

        if(CFURLGetFileSystemRepresentation(URLBundle, 1, (UInt8*)cheminResources, PATH_MAX))
            chdir(cheminResources);


        // Libération de la mémoire

        delete[] cheminResources;
        CFRelease(URLBundle);

    #else

        // Version d'OpenGL

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    #endif


    // Création de la fenêtre

    m_fenetre = SDL_CreateWindow(m_titreFenetre.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_largeurFenetre, m_hauteurFenetre, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if(m_fenetre == 0)
    {
        std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }


    // Création du contexte OpenGL

    m_contexteOpenGL = SDL_GL_CreateContext(m_fenetre);

    if(m_contexteOpenGL == 0)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_fenetre);
        SDL_Quit();

        return false;
    }

    return true;
}


bool SceneOpenGL::initGL()
{
    #ifdef WIN32

        // On initialise GLEW

        GLenum initialisationGLEW( glewInit() );


        // Si l'initialisation a échoué :

        if(initialisationGLEW != GLEW_OK)
        {
            // On affiche l'erreur grâce à la fonction : glewGetErrorString(GLenum code)

            std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString(initialisationGLEW) << std::endl;


            // On quitte la SDL

            SDL_GL_DeleteContext(m_contexteOpenGL);
            SDL_DestroyWindow(m_fenetre);
            SDL_Quit();

            return false;
        }

    #endif


    // Activation du Depth Buffer

    glEnable(GL_DEPTH_TEST);


    // Tout s'est bien passé, on retourne true

    return true;
}


void SceneOpenGL::bouclePrincipale()
{
    // Variables

    Uint32 frameRate (1000 / 50);
    Uint32 debutBoucle(0), finBoucle(0), tempsEcoule(0), frame(0);


    // Matrices

    mat4 projection;
    mat4 modelview;

    projection = perspective(70.0, (double) m_largeurFenetre / m_hauteurFenetre, 1.0, 100.0);
    modelview = mat4(1.0);

    // Caméra mobile

    Camera camera(vec3(5, 5, 4), vec3(-3, 0, 0), vec3(0, 1, 0), 0.5, 0.5);
    m_input.afficherPointeur(false);
    m_input.capturerPointeur(true);

    // Texture
    
    Texture texture("Textures/Herbe.jpg");
    texture.charger();

    
    // Shader
    
    Shader shaderTexture("Shaders/texture.vert", "Shaders/texture.frag");
    shaderTexture.charger();
    
    //Marche
    std::list<const char*> path;
    path.push_back("frame2.obj");
    path.push_back("frame1.obj");
    path.push_back("frame2.obj");
    path.push_back("frame3.obj");
    
    AnimatedOBJ elephant(path, "Shaders/texture.vert", "Shaders/texture.frag", "Textures/elephant1.jpg");
    
    //Debout
    path.clear();
    path.push_back("debout0.obj");
    path.push_back("debout1.obj");
    path.push_back("debout2.obj");
    path.push_back("debout3.obj");
    
    elephant.ajouterAnimation(path, "Shaders/texture.vert", "Shaders/texture.frag", "Textures/elephant1.jpg");
    elephant.charger();
    
    // Objet Caisse

    Caisse caisse(2.0, "Shaders/texture.vert", "Shaders/texture.frag", "Textures/Caisse2.jpg");
    caisse.charger();
    
    // Objet Sol
    
    Sol sol(20.0, "Shaders/texture.vert", "Shaders/texture.frag", "Textures/Herbe.jpg");
    sol.charger();
    
    //DAE MODEL
    //Model3D* model2 = MeshLoader::get().openModel("astroBoy.dae");//astroBoy.dae|robot.dae
    //if(model2) models.push_back(model2);

    //Repere
    
    //Repere Repere("Shaders/repere.vert", "Shaders/repere.frag");
    
    // Boucle principale
    int animation = 0;

    while(!m_input.terminer())
    {
        // On définit le temps de début de boucle

        debutBoucle = SDL_GetTicks();


        // Gestion des évènements

        m_input.updateEvenements();

        if(m_input.getTouche(SDL_SCANCODE_ESCAPE))
           break;

        if(m_input.getTouche(SDL_SCANCODE_W) && animation==0)
            animation = 1;
        if(m_input.getTouche(SDL_SCANCODE_E) && animation==0)
            animation = 2;
        
        
        camera.deplacer(m_input);


        // Nettoyage de l'écran

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Gestion de la caméra

        camera.lookAt(modelview);

        mat4 sauvegardeModelview = modelview;
        // Affichage du cube
        modelview = translate(modelview, vec3(0, 1, 0));

        //caisse.afficher(projection, modelview);
        
        modelview = sauvegardeModelview;
       
        modelview = scale(modelview, vec3(0.03,0.03,0.03));
        modelview = rotate(modelview, 90.0f, vec3(0,1,0));
        modelview = translate(modelview, vec3(-30, 3, -20));
        
        
        if(++frame>95)
        {
            frame=0;
            animation=0;
        }
        elephant.afficher(projection, modelview, frame, animation);

        // Restauration de la matrice
        
        modelview = sauvegardeModelview;
        
        // Affichage du cube
        
        sol.afficher(projection, modelview);

        
        // Actualisation de la fenêtre

        SDL_GL_SwapWindow(m_fenetre);


        // Calcul du temps écoulé

        finBoucle = SDL_GetTicks();
        tempsEcoule = finBoucle - debutBoucle;


        // Si nécessaire, on met en pause le programme

        if(tempsEcoule < frameRate)
            SDL_Delay(frameRate - tempsEcoule);
        //else
        //    frame++;
    }
}

