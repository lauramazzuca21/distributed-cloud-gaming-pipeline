#include <EGL/egl.h>

#include <math.h>
#include <stdio.h>
#include <sstream>
#include <stdexcept>

#include "Render.h"

static const EGLint configAttribs[] = {
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_DEPTH_SIZE, 8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE
};   

void assertOpenGLError(const std::string& msg) {
	GLenum error = glGetError();

	if (error != GL_NO_ERROR) {
		std::stringstream s;
		s << "OpenGL error 0x" << std::hex << error << " at " << msg;
		throw std::runtime_error(s.str());
	}
}

void assertEGLError(const std::string& msg) {
	EGLint error = eglGetError();

	if (error != EGL_SUCCESS) {
		std::stringstream s;
		s << "EGL error 0x" << std::hex << error << " at " << msg;
		throw std::runtime_error(s.str());
	}
}

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

    eglChooseConfig(eglDpy, configAttribs, &eglCfg, 1, &numConfigs);
	assertEGLError("eglChooseConfig");

    // 3. Bind the API
    eglBindAPI(EGL_OPENGL_API);
	assertEGLError("eglBindAPI");

    // 3. Create a context and make it current
    EGLContext eglCtx = eglCreateContext(eglDpy, eglCfg, EGL_NO_CONTEXT, NULL);
	assertEGLError("eglCreateContext");

    eglMakeCurrent(eglDpy, EGL_NO_SURFACE, EGL_NO_SURFACE, eglCtx);
	assertEGLError("eglMakeCurrent");

    glewInit();
    // from now on use your OpenGL context
    render::init();
    render::display();

    // 4. Terminate EGL when finished
    eglDestroyContext(eglDpy, eglCtx);
	assertEGLError("eglDestroyContext");
	
	eglTerminate(eglDpy);
	assertEGLError("eglTerminate");
    return 0;
}