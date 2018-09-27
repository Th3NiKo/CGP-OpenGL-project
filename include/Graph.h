#ifndef GRAPH_H
#define GRAPH_H
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

class Graph
{
    public:
        Graph(float res);
        virtual ~Graph();
    const float pi = 3.14f;
    int resolution;

    void DrawGraph(){

    }

    protected:

    private:
};

#endif // GRAPH_H
