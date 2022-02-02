#ifndef _RENDER_H_
#define _RENDER_H_

#include <stdlib.h>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.inl>

#include "Errors.h"
#include "Model.h"

class Render
{
    float aspect;

    GLuint fb, color, depth;
    GLuint programId, MatProj, MatModel, MatView;


    static const int	width = 800;
    static const int	height = 800;

    Model * model;
    glm::mat4 Projection, View;

    //CAMERA STUFF
    static const int    fov = 45;
    static constexpr float	zNear = 0.01f;
    static constexpr float	zFar = 10000.0f;
    glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 10.0);
    glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
    glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);

    int Index = 0; 

public:
    Render() { 
        init(); 
        }
    ~Render() {
        // glDeleteRenderbuffers(1, &depth);
        glDeleteRenderbuffers(1, &color);
        glDeleteFramebuffers(1, &fb);
    }
    void display();
private:
    void print_stats();
    void print_frame();
    void init();
    void initShader();
    void initBuffers();
};

#endif