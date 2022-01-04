#include "Render.h"
#include "Errors.h"

#include <EGL/egl.h>
#include <GL/glew.h>

#include <math.h>
#include <stdio.h>

static const std::array<EGLint, 13> context_attrib {
    EGL_CONTEXT_CLIENT_VERSION, 3,
    EGL_CONTEXT_MAJOR_VERSION, 4,
    EGL_CONTEXT_MINOR_VERSION, 5,
    EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT, //core profile gives segmentation fault in readPixels
    EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE, EGL_FALSE,
    EGL_CONTEXT_OPENGL_DEBUG, EGL_FALSE,
    EGL_NONE
};

static const std::array<EGLint, 13> config_Attrib {
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_DEPTH_SIZE, 8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE
};
int main(int argc, char *argv[])
{
    // 1. Initialize EGL
    EGLDisplay eglDpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assertEGLError("eglGetDisplay");

    EGLint major, minor;    
    eglInitialize(eglDpy, &major, &minor);
	assertEGLError("eglInitialize");

    // 2. Select an appropriate configuration
    EGLint numConfigs;
    EGLConfig eglCfg;

    eglChooseConfig(eglDpy, config_Attrib.data(), &eglCfg, 1, &numConfigs);
	assertEGLError("eglChooseConfig");

    // 3. Bind the API
    eglBindAPI(EGL_OPENGL_API);
	assertEGLError("eglBindAPI");

    // 3. Create a context and make it current
    EGLContext eglCtx = eglCreateContext(eglDpy, eglCfg, EGL_NO_CONTEXT, context_attrib.data());
	assertEGLError("eglCreateContext");

    eglMakeCurrent(eglDpy, EGL_NO_SURFACE, EGL_NO_SURFACE, eglCtx);
	assertEGLError("eglMakeCurrent");
    
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    
    std::cout << "Generate EGL context with version " << major << "." << minor << std::endl;
    std::cout << glGetString( GL_VERSION ) << std::endl;
    std::cout << glGetString( GL_VENDOR ) << std::endl;
    std::cout << glGetString( GL_RENDERER ) << std::endl;
    std::cout << glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl;

    assert(major >= 4);
    assert(minor >= 5);

    glewInit();

    // from now on use your OpenGL context
    render::init();
    int i = 0;
    while(i<10) {
        render::display();
        i++;
    }

    // 4. Terminate EGL when finished
    eglDestroyContext(eglDpy, eglCtx);
	assertEGLError("eglDestroyContext");
	
	eglTerminate(eglDpy);
	assertEGLError("eglTerminate");
    return 0;
}