#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.inl>

#include "../utils/glLogUtils.hpp"

#include "../graphics/ContextEGL.hpp"
#include "../graphics/ShaderProgram.hpp"

#include "Model.hpp"

class Renderer
{
    public:
    static const int	width = 1280;
    static const int	height = 720;
    float aspect = width/height*1.0f;
    int nDraw = 1;

    Renderer(bool useEGL = false) : _useEGL{useEGL} { 
        if(_useEGL)
        {
            _ctxt = new ContextEGL();
            _ctxt->init();
        }
        init(); 
    }
    ~Renderer() {
        glDeleteRenderbuffers(1, &_depth);
        glDeleteRenderbuffers(1, &_color);
        glDeleteFramebuffers(1, &_fb);
        
        if(_useEGL)
            _ctxt->terminate();
    }
    const std::vector<uint8_t>& nextFrameBuffer(double dt); 
    void draw(double dt);
    void addModel(Model *model);
private:

    GLuint _fb, _color, _depth;
 
    ContextEGL *_ctxt;
    bool _useEGL = false;
    std::vector<Model *> _models;
    std::map<Constants::ShadingType, ShaderProgram *> _loadedShaders = {};

    glm::mat4 _projection, _view;

    std::vector<uint8_t> _pixels = std::vector<uint8_t>(width * height * 4);
    const std::vector<uint8_t>& getPixels(); //method to change so that it uses memsh

    void init();
    void initBuffers();
    

};
