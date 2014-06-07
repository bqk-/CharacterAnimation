//
//  AnimatedOBJ.cpp
//  OpenGL
//
//  Created by Thibault Miclo on 30/05/2014.
//
//
//
//  ModelOBJ.cpp
//  OpenGL
//
//  Created by Thibault Miclo on 20/05/2014.
//
//

#include "AnimatedOBJ.h"

AnimatedOBJ::AnimatedOBJ(std::list<const char*> path, std::string const vertexShader, std::string const fragmentShader, std::string const texture){

    obj=NULL;
    std::list<const char*>::const_iterator
    lit (path.begin()),
    lend(path.end());
    obj =new ModelOBJ(*path.begin(),vertexShader,fragmentShader,texture);
    int i=0, interval = 24;
    ModelOBJ* n = NULL;
    std::vector<glm::vec3> new_vertices;
    for(;lit!=lend;++lit)
    {
        ModelOBJ o(*lit,vertexShader,fragmentShader,texture);
        std::cout << *lit << std::endl;
        vertices_tab.push_back(o.getVertices());

        if(++lit!=lend)
        {
            std::cout << *lit << std::endl;
            n = new ModelOBJ(*lit,vertexShader,fragmentShader,texture);
            
        }
        else
        {
            std::cout << *path.begin() << std::endl;
            n = new ModelOBJ(*path.begin(),vertexShader,fragmentShader,texture);
        }
        lit--;
        i=1;
        while(i<interval)
        {
            std::cout << "Interpolation " << i << "/" << interval  << std::endl;
            new_vertices = interpole_sec(o.getVertices(),n->getVertices(),i,interval);
            vertices_tab.push_back(new_vertices);
            i++;
        }
    }
    animations_vertices.push_back(vertices_tab);
    std::cout << "Frames : " << vertices_tab.size() << std::endl;
};

void AnimatedOBJ::ajouterAnimation(std::list<const char*> path, std::string const vertexShader, std::string const fragmentShader, std::string const texture){
    vertices_tab.clear();
    std::list<const char*>::const_iterator
    lit (path.begin()),
    lend(path.end());
    int i=0, interval = 24;
    ModelOBJ* n = NULL;
    std::vector<glm::vec3> new_vertices;
    for(;lit!=lend;++lit)
    {
        ModelOBJ o(*lit,vertexShader,fragmentShader,texture);
        std::cout << *lit << std::endl;
        vertices_tab.push_back(o.getVertices());
        
        if(++lit!=lend)
        {
            std::cout << *lit << std::endl;
            n = new ModelOBJ(*lit,vertexShader,fragmentShader,texture);
            
        }
        else
        {
            std::cout << *path.begin() << std::endl;
            n = new ModelOBJ(*path.begin(),vertexShader,fragmentShader,texture);
        }
        lit--;
        i=1;
        while(i<interval)
        {
            std::cout << "Interpolation " << i << "/" << interval  << std::endl;
            new_vertices = interpole_sec(o.getVertices(),n->getVertices(),i,interval);
            vertices_tab.push_back(new_vertices);
            i++;
        }
    }
    animations_vertices.push_back(vertices_tab);
    std::cout << "Frames : " << vertices_tab.size() << std::endl;
}

void AnimatedOBJ::charger()
{
   obj->charger();
}

void AnimatedOBJ::afficher(glm::mat4 &projection, glm::mat4 &modelview, int frame, int animation)
{
    if(animation==0)
        obj->recharger(animations_vertices[0][0]);
    else
        obj->recharger(animations_vertices[animation-1][frame]);
    obj->afficher(projection, modelview);
}


std::vector<glm::vec3> AnimatedOBJ::interpole_sec(std::vector<glm::vec3> a, std::vector<glm::vec3> b, int j, int interval)
{
    glm::vec3 tmp;
    std::vector<glm::vec3> result;
    for(int i = 0; i < a.size(); i++) {
        if(a[i].x != b[i].x || a[i].y != b[i].y || a[i].z != b[i].z)
        {
            tmp.x=a[i].x+(j*((b[i].x-a[i].x)/interval));
            tmp.y=a[i].y+(j*((b[i].y-a[i].y)/interval));
            tmp.z=a[i].z+(j*((b[i].z-a[i].z)/interval));
            result.push_back(tmp);
        }
        else
        {
            result.push_back(a[i]);
        }
    }
    return result;
}