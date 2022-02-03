#ifndef _RENDER_H_
#define _RENDER_H_

#include <stdlib.h>
#include <stdio.h>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.inl>

#include "LogUtils.h"
#include "Dragon.h"
#include "ShaderProgram.h"
#include "Light.h"

class Render
{
    float aspect;

    GLuint fb, color, depth;

    static const int	width = 1920;
    static const int	height = 1200;

    Light * light = new Light();
    Dragon * model;
    std::map<Constants::ShadingType, ShaderProgram *> loadedShaders = {};

    glm::mat4 Projection, View;

    //CAMERA STUFF
    static const int fov = 45;
    static constexpr float	zNear = 0.01f;
    static constexpr float	zFar = 10000.0f;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    int Index = 0; 

public:
    Render() { 
        init(); 
        }
    ~Render() {
        glDeleteRenderbuffers(1, &depth);
        glDeleteRenderbuffers(1, &color);
        glDeleteFramebuffers(1, &fb);
    }
    void display();
private:
    void printFrame();
    void init();
    void initBuffers();
};

#endif