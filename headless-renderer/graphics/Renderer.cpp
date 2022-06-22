#include "Renderer.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../extern/images/stb_image_write.h"

const std::vector<uint8_t>& Renderer::nextFrameBuffer(double dt) {
    draw(dt);
    return getPixels();
}

void Renderer::addModel(Model *model){
    _models.push_back(model);

    if (_loadedShaders.find(model->getShaderType()) == _loadedShaders.end()) {
        ShaderProgram * current = new ShaderProgram(model->getShaderType());
        current->enable();
        // current->setUniformVector3("light_color_pointer", _light->getColor());
        // current->setUniformFloat("light_power_pointer", _light->getPower());
        current->disable();
        _loadedShaders.emplace(current->getType(), current);
    }

}

const std::vector<uint8_t>& Renderer::getPixels() {
    // gl::log::debug::print("Reading pixels...");
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, _pixels.data());

    for(int line = 0; line != height/2; ++line) {
        std::swap_ranges(
                _pixels.begin() + 4 * width * line,
                _pixels.begin() + 4 * width * (line+1),
                _pixels.begin() + 4 * width * (height-line-1));
    }
    gl::log::errors::assertOpenGLError("glReadPixels");
    // gl::log::debug::print("Read %lu pixels.\n", pixels.size());

    return _pixels;
}

void Renderer::initBuffers() {
    //correct setup thanks to https://github.com/cirosantilli/cpp-cheat/blob/70b22ac36f92e93c94f951edb8b5af7947546525/opengl/offscreen.c
    gl::log::debug::print("\nFRAMEBUFFER...");
//1. Generate framebuffer to hold rendering destination
    glGenFramebuffers(1, &_fb);
    glBindFramebuffer(GL_FRAMEBUFFER, _fb);
    gl::log::debug::print("Done.\n");
//2. Generate color render buffer
    gl::log::debug::print("RENDERBUFFER...");
    glGenRenderbuffers(1, &_color);
    glBindRenderbuffer(GL_RENDERBUFFER, _color);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
    gl::log::errors::assertOpenGLError("glRenderbufferStorage");
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _color);
    gl::log::errors::assertOpenGLError("glFramebufferRenderbuffer");
    gl::log::debug::print("Done.\n");
//3. Generate depth render buffer with 32 bit component to handle alpha as well
    gl::log::debug::print("DEPTHBUFFER...");
    glGenRenderbuffers(1, &_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, _depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth);
    gl::log::debug::print("Done.\n");
//4.
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    gl::log::errors::assertOpenGLError("glReadBuffer");
}

void Renderer::init() {
    gl::log::debug::print("Init GLEW\n");
    glewInit();

    if (_useEGL) {
        gl::log::debug::print("Init buffers...");
        initBuffers();
        gl::log::debug::print("Done.\n");
    }

    gl::log::debug::print("Done.\n Setting up all dragons...");
    // float scale = 0.05f;
    // float pos = 250.0f;
    // for (int i = 0; i < _models.size(); i++)
    // {
        // scale *= 1.05f;
        // pos -= 50.0f;
        //moved into state/GameObject
        // dragons.at(i)->scaleOCS(glm::vec3(scale));
        // dragons.at(i)->translateOCS(glm::vec3(pos, 0.0f, -150.0f));
        // dragons_top.at(i)->scaleOCS(glm::vec3(scale));
        // dragons_top.at(i)->translateOCS(glm::vec3(pos, 150.0f, -150.0f));
        // dragons_bottom.at(i)->scaleOCS(glm::vec3(scale));
        // dragons_bottom.at(i)->translateOCS(glm::vec3(pos, -150.0f, -150.0f));
       
    // }
    gl::log::debug::print("Done.\n");
    

//5. setup background
	glClearColor(1.0, 1.0, 1.0, 1.0);
    glViewport(0, 0, width, height);

//6. setup OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);	
    glEnable(GL_LINE_SMOOTH);

    //glPixelStorei sets pixel storage modes that affect the operation of subsequent glReadPixels
    // as well as the unpacking of texture patterns (see glTexImage2D and glTexSubImage2D). 
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    gl::log::printStats();
    gl::log::errors::checkFramebufferStatus(); //this check should be done outside of the constructor, maybe calling init in a second moment
                                               //that's because, by cppiso, errors happening in the constructor should be handled by exceptions
}

void Renderer::draw(double dt) {
//----more state stuff
    // static double millisec = 0.0;
    // millisec += dt;

    // if (millisec > 10000.0 && nDraw < MAX_DRAGONS_PER_ROW) //10 seconds have passed, let's increase rendered dragons
    // {
    //     nDraw += 1;
    //     millisec = 0.0;
    // }
//--------------------
    // _projection = _camera->getProjectionMatrix(width, height);
	// _view = _camera->getViewMatrix();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //this draw implementation is obviously too case-specific, needs to change once the
    //dragons become just a collection of Models in something like this
    /*
        for (Model& m : models)
        {
            ShaderProgram *currentShader = loadedShaders[m->getShaderType()];
    
            currentShader->enable();
            currentShader->setUniformVector3("light_position_pointer", light->getPosition());

            m->draw(currentShader, View, Projection);
            currentShader->disable();
        }
    */
    for (int i = 0; i < _models.size(); i++)
    {
        ShaderProgram * currentShader = _loadedShaders[_models.at(i)->getShaderType()];
    
        currentShader->enable();
        // currentShader->setUniformVector3("light_position_pointer", _light->getPosition());

        _models.at(i)->draw(currentShader, _view, _projection);

        currentShader->disable();

    }

    glFinish();
    gl::log::errors::assertOpenGLError("glFinish");

}
