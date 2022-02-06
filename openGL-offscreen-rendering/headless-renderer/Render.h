#ifndef _RENDER_H_
#define _RENDER_H_

#include <stdlib.h>
#include <stdio.h>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.inl>

#include "utils/glLogUtils.h"

#include "graphics/ContextEGL.h"
#include "graphics/ShaderProgram.h"
#include "graphics/Light.h"
#include "graphics/Camera.h"

#include "Dragon.h"



class Render
{

    GLuint fb, color, depth;

    std::vector<uint8_t> pixels;

    ContextEGL* ctxt = new ContextEGL();

    Light * light = new Light();
    Camera * camera = new Camera();
    Dragon * model;
    std::map<Constants::ShadingType, ShaderProgram *> loadedShaders = {};

    glm::mat4 Projection, View;
private:
    const std::vector<uint8_t>& getPixels();
    void init();
    void initBuffers();
    
public:
    static const int	width = 1920;
    static const int	height = 1200;
    float aspect = width/height*1.0f;


    Render() { 
        ctxt->init();
        init(); 
        }
    ~Render() {
        glDeleteRenderbuffers(1, &depth);
        glDeleteRenderbuffers(1, &color);
        glDeleteFramebuffers(1, &fb);

        ctxt->terminate();
    }
    const std::vector<uint8_t>& nextFramePixels();
};

#endif