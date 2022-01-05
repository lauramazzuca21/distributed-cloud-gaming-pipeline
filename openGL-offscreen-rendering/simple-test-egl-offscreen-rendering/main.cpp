#include "Render.h"
#include "Errors.h"

#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <EGL/eglext.h>
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

EGLBoolean platform_specific_EGLdisplay(EGLDisplay * eglDpy, EGLint * numConfigs, EGLConfig * eglCfg, EGLint * major, EGLint  * minor)
{
    //this display initializatoin is done to look for available displays when
    //deploying to machines with unknown HW configs
    static const int MAX_DEVICES = 32;
    EGLDeviceEXT eglDevs[MAX_DEVICES];
    EGLint numDevices;

    PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT = (PFNEGLQUERYDEVICESEXTPROC) eglGetProcAddress("eglQueryDevicesEXT");

    eglQueryDevicesEXT(MAX_DEVICES, eglDevs, &numDevices);

    printf("Detected %d devices\n", numDevices);

    PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT = (PFNEGLGETPLATFORMDISPLAYEXTPROC) eglGetProcAddress("eglGetPlatformDisplayEXT");

    // 1. Initialize EGL && Select an appropriate configuration
    EGLBoolean found = EGL_FALSE;
    for( EGLint i = 0; (EGL_FALSE == found) && (i < numDevices); ++i ){
        (*eglDpy) = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, eglDevs[i], 0);
        eglInitialize(eglDpy, major, minor);
        found = eglChooseConfig((*eglDpy), config_Attrib.data(), eglCfg, 1, numConfigs);
        printf("device %i - config result %d\n", i, found ? 1 : 0);
    }

    return found;	
}

int main(int argc, char *argv[])
{
    EGLDisplay eglDpy;
	EGLint numConfigs;
    EGLConfig eglCfg;
    EGLint major, minor;    
    // 1. Initialize EGL & 2. Select an appropriate configuration
    if (platform_specific_EGLdisplay(&eglDpy, &numConfigs, &eglCfg, &major, &minor) != EGL_TRUE)
    {
        eglDpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        assertEGLError("eglGetDisplay");
        eglInitialize(eglDpy, &major, &minor);
        assertEGLError("eglInitialize");
        eglChooseConfig(eglDpy, config_Attrib.data(), &eglCfg, 1, &numConfigs);
	    assertEGLError("eglChooseConfig");
    }
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