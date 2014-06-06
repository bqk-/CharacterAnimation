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
    
    std::list<const char*>::const_iterator
    lit (path.begin()),
    lend(path.end());
    //int i=0, interval = 24;
    //ModelOBJ *n=NULL;
    for(;lit!=lend;++lit)
    {
        ModelOBJ o(*lit,vertexShader,fragmentShader,texture);
        //ModelOBJ n();
        std::cout << *lit << std::endl;
        frames.push_back(o);
        /*
        if(++lit!=lend)
        {
            std::cout << *lit << std::endl;
            n= new ModelOBJ(*lit,vertexShader,fragmentShader,texture);
            
        }
        else
        {
            std::cout << *path.begin() << std::endl;
            n= new ModelOBJ(*path.begin(),vertexShader,fragmentShader,texture);
        }
        lit--;
        i=0;
        while(i<interval)
        {
            std::cout << "Interpolation " << i << "/" << interval  << std::endl;
            std::vector<glm::vec3> new_vertices = interpole_sec(o.getVertices(),n->getVertices(),i,interval);
            ModelOBJ t(new_vertices,o,vertexShader,fragmentShader,texture);
            frames.push_back(t);
            i++;
        }
         */
    }
    frames.push_back(frames[0]);
    THE_FRAME = frames.size();
   
};


void AnimatedOBJ::charger()
{
    //frames[0].charger();
    
      for(int i = 0; i != frames.size(); i++) {
         frames[i].charger();
     }
      
}

void AnimatedOBJ::afficher(glm::mat4 &projection, glm::mat4 &modelview, int frame)
{
    /*
    std::cout << "Affichage Frame " << frame%frames.size() << std::endl;
    int prev=0,next=0;
    std::vector<glm::vec3> tmp_vertices;
    if(frame%24==0)
    {
        frames[THE_FRAME].updateVBO(frames[frame%24].getVerticesAddr(), frames[frame%24].getVertices().size() * sizeof(glm::vec3), 0);
    }
    else
    {
        do{
            frame--;
        }
        while(frame%24==0);
        prev=frame%24;
        next=prev+1;
        if(next >= frames.size())
            next=0;
        tmp_vertices=interpole_sec(frames[prev].getVertices(), frames[prev+1].getVertices(), frame, 24);
        frames[THE_FRAME].updateVBO(&tmp_vertices, frames[frame%24].getVertices().size() * sizeof(glm::vec3), 0);
    }
    frames[THE_FRAME].afficher(projection, modelview);
     */
    frames[frame].afficher(projection, modelview);
}


std::vector<glm::vec3> AnimatedOBJ::interpole_sec(std::vector<glm::vec3> a, std::vector<glm::vec3> b, int j, int interval)
{
    std::cout << "Interpolation " << j << "/" << interval  << std::endl;
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