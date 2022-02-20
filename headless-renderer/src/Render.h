#ifndef _RENDER_H_
#define _RENDER_H_

#include <stdlib.h>
#include <stdio.h>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.inl>

#include "../utils/glLogUtils.h"

#include "../graphics/ContextEGL.h"
#include "../graphics/ShaderProgram.h"
#include "../graphics/Light.h"
#include "../graphics/Camera.h"

#include "Dragon.h"



class Render
{
    public:
    static const int	width = 1280;
    static const int	height = 720;
    float aspect = width/height*1.0f;


    Render(bool useEGL = false) : _useEGL{useEGL} { 
        if(_useEGL)
            _ctxt->init();
        init(); 
    }
    ~Render() {
        glDeleteRenderbuffers(1, &depth);
        glDeleteRenderbuffers(1, &color);
        glDeleteFramebuffers(1, &fb);
        if(_useEGL)
            _ctxt->terminate();
    }
    const std::vector<uint8_t>& nextFrameAndGetPixels();
    void nextFrame();

private:

    GLuint fb, color, depth;

    std::vector<uint8_t> pixels = std::vector<uint8_t>(width * height * 4);

    ContextEGL* _ctxt;
    bool _useEGL = false;
    Light * light = new Light();
    Camera * camera = new Camera();
    Dragon * model;
    std::map<Constants::ShadingType, ShaderProgram *> loadedShaders = {};

    glm::mat4 Projection, View;


    const std::vector<uint8_t>& getPixels();
    void init();
    void initBuffers();
    

};

#endif