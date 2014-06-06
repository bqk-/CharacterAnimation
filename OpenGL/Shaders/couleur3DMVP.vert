// Version du GLSL

#version 150 core


// Entrées

in vec3 in_Vertex;
in vec3 in_Color;


// Uniform

uniform mat4 modelviewProjection;


// Sortie

out vec3 color;


// Fonction main

void main()
{
    // Position finale du vertex en 3D

    gl_Position = modelviewProjection * vec4(in_Vertex, 1.0);


    // Envoi de la couleur au Fragment Shader

    color = in_Color;
}
