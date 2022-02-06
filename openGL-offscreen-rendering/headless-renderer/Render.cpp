#include "Render.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "extern/stb_image_write.h"

const std::vector<uint8_t>& Render::getPixels() {
    // static int i=0;

    printf("Reading pixels...");
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    for(int line = 0; line != height/2; ++line) {
        std::swap_ranges(
                pixels.begin() + 4 * width * line,
                pixels.begin() + 4 * width * (line+1),
                pixels.begin() + 4 * width * (height-line-1));
    }
    gl::log::errors::assertOpenGLError("glReadPixels");

    return pixels;
}

void Render::initBuffers() {
    //correct setup thanks to https://github.com/cirosantilli/cpp-cheat/blob/70b22ac36f92e93c94f951edb8b5af7947546525/opengl/offscreen.c
//1. Generate framebuffer to hold rendering destination
    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
//2. Generate color render buffer
    glGenRenderbuffers(1, &color);
    glBindRenderbuffer(GL_RENDERBUFFER, color);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
    gl::log::errors::assertOpenGLError("glRenderbufferStorage");
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color);
    gl::log::errors::assertOpenGLError("glFramebufferRenderbuffer");
//3. Generate depth render buffer with 32 bit component to handle alpha as well
    glGenRenderbuffers(1, &depth);
    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
//4.
    glReadBuffer(GL_COLOR_ATTACHMENT0);
}

void Render::init() {
    glewInit();

    pixels.reserve(width * height * 4);

    initBuffers();

    model = new Dragon("xyzrgb_dragon", Constants::ShadingType::PHONG);

    if (loadedShaders.find(model->getShaderType()) == loadedShaders.end()) {
        ShaderProgram * current = new ShaderProgram(model->getShaderType());
        current->enable();
        current->setUniformVector3("light_color_pointer", light->getColor());
        current->setUniformFloat("light_power_pointer", light->getPower());
        current->disable();
        loadedShaders.emplace(current->getType(), current);
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

const std::vector<uint8_t>& Render::nextFramePixels() {

	Projection = camera->getProjectionMatrix(width, height);
	View = camera->getViewMatrix();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    ShaderProgram * currentShader = loadedShaders[model->getShaderType()];
    
    currentShader->enable();
    currentShader->setUniformVector3("light_position_pointer", light->getPosition());

    model->draw(currentShader, View, Projection);

    currentShader->disable();

    glFinish();
    gl::log::errors::assertOpenGLError("glFinish");


    return getPixels();
}