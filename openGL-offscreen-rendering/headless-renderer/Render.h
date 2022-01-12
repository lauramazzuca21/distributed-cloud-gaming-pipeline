#ifndef _RENDER_H_
#define _RENDER_H_

/*
 * OpenCV for saving the render target as an image file.
 */
#include <opencv2/opencv.hpp>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <random>

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "Errors.h"
// #include "camera.h"

#include <glm/glm.hpp>

class Render
{
    float aspect;

    GLuint fb, color, depth;
    GLuint programId, MatProj, MatModel, MatView;

    static const int NumVertices = 36;

    typedef  glm::vec4  point4;
    typedef  glm::vec4  color4;
    point4  vPositions[NumVertices];
    color4  vColors[NumVertices];
    glm::mat4 Projection, Model, View;
    // Camera * camera = new Camera();

    static const int	width = 800;
    static const int	height = 800;
    static const int    fov = 45;
    static constexpr float	zNear = 0.01f;
    static constexpr float	zFar = 10000.0f;


    point4 positions[8] = {
        point4(-0.5, -0.5,  0.5, 0.5),
        point4(-0.5,  0.5,  0.5, 0.5),
        point4(0.5,  0.5,  0.5, 0.5),
        point4(0.5, -0.5,  0.5, 0.5),
        point4(-0.5, -0.5, -0.5, 0.5),
        point4(-0.5,  0.5, -0.5, 0.5),
        point4(0.5,  0.5, -0.5, 0.5),
        point4(0.5, -0.5, -0.5, 0.5)
    };

    color4 colors[8] = {
        color4(1.0, 1.0, 1.0, 0.5),  // white
        color4(1.0, 0.0, 0.0, 0.5),  // red
        color4(1.0, 1.0, 0.0, 0.5),  // yellow
        color4(0.0, 1.0, 0.0, 0.5),  // green
        color4(0.0, 0.0, 1.0, 0.5),  // blue
        color4(1.0, 0.0, 1.0, 0.5),  // magenta
        color4(0.0, 0.0, 0.0, 0.5),  // black
        color4(0.0, 1.0, 1.0, 0.5)   // cyan
    };
    int Index = 0; 

public:
    Render() { init(); }
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
    void polygon_multicolor(int a, int b, int c, int d);
    void polygon_monocolor(int a, int b, int c, int d);
    void Cube();

};

#endif