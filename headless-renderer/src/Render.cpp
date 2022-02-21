#include "Render.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../extern/stb_image_write.h"

const std::vector<uint8_t>& Render::getPixels() {
    // gl::log::debug::print("Reading pixels...");
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    for(int line = 0; line != height/2; ++line) {
        std::swap_ranges(
                pixels.begin() + 4 * width * line,
                pixels.begin() + 4 * width * (line+1),
                pixels.begin() + 4 * width * (height-line-1));
    }
    gl::log::errors::assertOpenGLError("glReadPixels");
    // gl::log::debug::print("Read %lu pixels.\n", pixels.size());

    return pixels;
}

void Render::initBuffers() {
    //correct setup thanks to https://github.com/cirosantilli/cpp-cheat/blob/70b22ac36f92e93c94f951edb8b5af7947546525/opengl/offscreen.c
    gl::log::debug::print("\nFRAMEBUFFER...");
//1. Generate framebuffer to hold rendering destination
    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
    gl::log::debug::print("Done.\n");
//2. Generate color render buffer
    gl::log::debug::print("RENDERBUFFER...");
    glGenRenderbuffers(1, &color);
    glBindRenderbuffer(GL_RENDERBUFFER, color);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
    gl::log::errors::assertOpenGLError("glRenderbufferStorage");
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color);
    gl::log::errors::assertOpenGLError("glFramebufferRenderbuffer");
    gl::log::debug::print("Done.\n");
//3. Generate depth render buffer with 32 bit component to handle alpha as well
    gl::log::debug::print("DEPTHBUFFER...");
    glGenRenderbuffers(1, &depth);
    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
    gl::log::debug::print("Done.\n");
//4.
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    gl::log::errors::assertOpenGLError("glReadBuffer");
}

void Render::init() {
    gl::log::debug::print("Init GLEW\n");
    glewInit();

    if (_useEGL) {
        gl::log::debug::print("Init buffers...");
        initBuffers();
        gl::log::debug::print("Done.\n");
    }

    dragons.push_back(new Dragon("xyzrgb_dragon_1"));
    dragons.push_back(new Dragon("xyzrgb_dragon_2", Constants::MaterialType::BRASS));
    dragons.push_back(new Dragon("xyzrgb_dragon_3", Constants::MaterialType::RED_PLASTIC));
    dragons.push_back(new Dragon("xyzrgb_dragon_4", Constants::MaterialType::SLATE));
    dragons.push_back(new Dragon("xyzrgb_dragon_5", Constants::MaterialType::YELLOW_RUBBER));

    float scale = 0.01f;
    float pos = 100.0f;
    for (auto dragon : dragons)
    {
        scale *= 1.5f;
        pos -= 200.0f;
        dragon->scaleOCS(glm::vec3(scale));
        dragon->translateOCS(glm::vec3(0.0f, 0.0f, pos));
        if (loadedShaders.find(dragon->getShaderType()) == loadedShaders.end()) {
            ShaderProgram * current = new ShaderProgram(dragon->getShaderType());
            current->enable();
            current->setUniformVector3("light_color_pointer", light->getColor());
            current->setUniformFloat("light_power_pointer", light->getPower());
            current->disable();
            loadedShaders.emplace(current->getType(), current);
        }
    }
    

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
    gl::log::errors::checkFramebufferStatus();
}

const std::vector<uint8_t>& Render::nextFrameAndGetPixels(double dt) {
    nextFrame(dt);
    return getPixels();
}

void Render::nextFrame(double dt) {
    static double sec = 0.0;
    static int nDraw = 1;
    sec += dt;
    if (sec > 60.0) //a minute passed, let's increase rendered dragons
    {
        nDraw += 1;
        if (nDraw > dragons.size())
        {
            nDraw = dragons.size();
        }
        sec = 0.0;
    }
    Projection = camera->getProjectionMatrix(width, height);
	View = camera->getViewMatrix();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    for (int i = 0; i < nDraw; i++)
    {
        ShaderProgram * currentShader = loadedShaders[dragons[i]->getShaderType()];
    
        currentShader->enable();
        currentShader->setUniformVector3("light_position_pointer", light->getPosition());

        dragons[i]->draw(currentShader, View, Projection);

        currentShader->disable();

    }

    glFinish();
    gl::log::errors::assertOpenGLError("glFinish");

}