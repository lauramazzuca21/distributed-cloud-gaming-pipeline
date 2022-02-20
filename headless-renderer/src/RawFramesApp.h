#ifndef __RAW_FRAMES_APP_H__
#define __RAW_FRAMES_APP_H__

#include "Render.h"
// #define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

class RawFramesApp {

private:
    Render* _render;
    GLFWwindow* _window;

    static void exit_sighandler (int signum);

    void init();


public:
    void run(int argc, char *argv[]);
    RawFramesApp() {
        init();
    }

};

#endif
