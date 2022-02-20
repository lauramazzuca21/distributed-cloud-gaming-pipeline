#include "RawFramesApp.h"
#include <sstream>
#include <csignal>

void RawFramesApp::exit_sighandler (int signum)
{
  printf ("Caught signal, close _window to stop\n");
}

void RawFramesApp::init() {
    signal (SIGINT, RawFramesApp::exit_sighandler);
    signal (SIGTERM, RawFramesApp::exit_sighandler);
    printf ("Initializing GLFW\n");
    if(!glfwInit()){    //init error
        printf ("ERROR: can't initialize GLFW\n");
        return;
    }
      printf ("Creating GLFW _window\n");
    _window = glfwCreateWindow(_render->width, _render->height, "Dragon", NULL, NULL);
    
    if(!_window){     //create _window fail
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(_window);
    printf ("Window loaded\n");

    _render = new Render();

}

void RawFramesApp::run(int argc, char *argv[]) {
    int frameCount = 0;
    double previousTime = glfwGetTime();
    while (!glfwWindowShouldClose(_window)) {  //close _window

        _render->nextFrame();
        frameCount++;
        double currentTime = glfwGetTime();
        // If a second has passed.
        if ( currentTime - previousTime >= 1.0 )
        {
            // Display the frame count here any way you want.
            gl::log::debug::print("FPS: %d\n", frameCount);

            frameCount = 0;
            previousTime = currentTime;
        }
        glfwSwapBuffers(_window); //swap buffer
        glfwPollEvents();
    }
    glfwDestroyWindow(_window);
    glfwTerminate();
    return;

}