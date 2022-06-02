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
    static const int	MAX_DRAGONS_PER_ROW = 10;
    float aspect = width/height*1.0f;
    int nDraw = 1;

    ~Render() {
        glDeleteRenderbuffers(1, &depth);
        glDeleteRenderbuffers(1, &color);
        glDeleteFramebuffers(1, &fb);
    }
    void init(bool isUsingEGLContext);
    void update(double dt);

    const std::vector<uint8_t>& nextFrameAndGetPixels(double dt);
    void nextFrame(double dt);
private:

    GLuint fb, color, depth;

    std::vector<uint8_t> pixels = std::vector<uint8_t>(width * height * 4);

    Light * light = new Light();
    Camera * camera = new Camera();
    std::vector<Dragon *> dragons;
    std::vector<Dragon *> dragons_top;
    std::vector<Dragon *> dragons_bottom;
    std::map<Constants::ShadingType, ShaderProgram *> loadedShaders = {};

    glm::mat4 Projection, View;


    const std::vector<uint8_t>& getPixels();
    void initBuffers();
    

};

#endif
