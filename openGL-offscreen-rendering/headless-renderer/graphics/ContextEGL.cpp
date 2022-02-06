#include "ContextEGL.h"

const EGLint ContextEGL::context_attrib[13] = {
    EGL_CONTEXT_CLIENT_VERSION, 3,
    EGL_CONTEXT_MAJOR_VERSION, 4,
    EGL_CONTEXT_MINOR_VERSION, 5,
    EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT, //core profile gives segmentation fault in readPixels
    EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE, EGL_FALSE,
    EGL_CONTEXT_OPENGL_DEBUG, EGL_FALSE,
    EGL_NONE
};

const EGLint ContextEGL::config_Attrib[13] = {
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_DEPTH_SIZE, 8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE
};

void ContextEGL::terminate()
{
    printf("Terminating... \n");  
    // 4. Terminate EGL when finished
    if (eglCtx != EGL_NO_CONTEXT)
    {
        eglDestroyContext(eglDpy, eglCtx);
	    gl::log::errors::assertEGLError("eglDestroyContext");
    }
	
    if (eglDpy != EGL_NO_DISPLAY)
    {
        eglTerminate(eglDpy);
        gl::log::errors::assertEGLError("eglTerminate");
    }
}

EGLBoolean ContextEGL::platformSpecificEGLdisplay(EGLDisplay * eglDpy, EGLint * numConfigs, EGLConfig * eglCfg, EGLint * major, EGLint  * minor)
{
    //this display initializatoin is done to look for available displays when
    //deploying to machines with unknown HW configs
    static const int MAX_DEVICES = 32;
    EGLDeviceEXT eglDevs[MAX_DEVICES];
    EGLint numDevices;

    printf("Detecting platform specific devices... ");

    PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT = (PFNEGLQUERYDEVICESEXTPROC) eglGetProcAddress("eglQueryDevicesEXT");

    eglQueryDevicesEXT(MAX_DEVICES, eglDevs, &numDevices);

    printf("Detected %d devices\n", numDevices);

    PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT = (PFNEGLGETPLATFORMDISPLAYEXTPROC) eglGetProcAddress("eglGetPlatformDisplayEXT");

    // 1. Initialize EGL && Select an appropriate configuration
    EGLBoolean found = EGL_FALSE;
    for( EGLint i = 0; (EGL_FALSE == found) && (i < numDevices); ++i ){
        (*eglDpy) = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, eglDevs[i], 0);
        found =  eglInitialize(eglDpy, major, minor); 
        printf("device %i - initialize result %d\n", i, found ? 1 : 0);
    }

    return found;	
}

void ContextEGL::init()
{
    try {
        printf("Initializing EGL display and EGL configuration... \n");  
        // 1. Initialize EGL & 2. Select an appropriate configuration
        if (platformSpecificEGLdisplay(&eglDpy, &numConfigs, &eglCfg, &major, &minor) != EGL_TRUE)
        {
            eglDpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
            gl::log::errors::assertEGLError("eglGetDisplay");
            eglInitialize(eglDpy, &major, &minor);
            gl::log::errors::assertEGLError("eglInitialize");
            printf("Done with EGL_DEFAULT_DISPLAY\n\n");
        }
        else
        {
            printf("Done with PLATFORM SPECIFIC DISPLAY\n\n");
        }

        eglChooseConfig(eglDpy, ContextEGL::config_Attrib, &eglCfg, 1, &numConfigs);
        gl::log::errors::assertEGLError("eglChooseConfig");

        // 3. Bind the API
        eglBindAPI(EGL_OPENGL_API);
        gl::log::errors::assertEGLError("eglBindAPI");

        // 3. Create a context and make it current
        eglCtx = eglCreateContext(eglDpy, eglCfg, EGL_NO_CONTEXT, ContextEGL::context_attrib);
        gl::log::errors::assertEGLError("eglCreateContext");
        /* create an EGL pbuffer surface */
        // surface = eglCreatePbufferSurface(eglDpy, eglCfg, surface_attrib);
        // gl::log::errors::assertEGLError("eglCreatePbufferSurface");
        eglMakeCurrent(eglDpy, EGL_NO_SURFACE, EGL_NO_SURFACE, eglCtx);
        gl::log::errors::assertEGLError("eglMakeCurrent");
    } catch(const std::exception& e) {
        printf("ERROR %s\n\n TERMINATE\n", e.what());
        terminate();
    }
    
    gl::log::print_EGL_info(major, minor);
}