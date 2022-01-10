#include "Render.h"

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

static const EGLint streamAttrFIFOMode[] = { 
    EGL_STREAM_FIFO_LENGTH_KHR, 4,
    EGL_METADATA0_SIZE_NV, 16*1024,
    EGL_METADATA1_SIZE_NV, 16*1024,
    EGL_METADATA2_SIZE_NV, 16*1024,
    EGL_METADATA3_SIZE_NV, 16*1024, EGL_NONE 
};

void terminate(EGLDisplay * eglDpy, EGLContext * eglCtx)
{
    // 4. Terminate EGL when finished
    if (*eglCtx != EGL_NO_CONTEXT)
    {
        eglDestroyContext(*eglDpy, *eglCtx);
	    errors::assertEGLError("eglDestroyContext");
    }
	
    if (*eglDpy != EGL_NO_DISPLAY)
    {
        eglTerminate(*eglDpy);
        errors::assertEGLError("eglTerminate");
    }
}

EGLBoolean platform_specific_EGLdisplay(EGLDisplay * eglDpy, EGLint * numConfigs, EGLConfig * eglCfg, EGLint * major, EGLint  * minor)
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
        found =  eglInitialize(eglDpy, major, minor); //eglChooseConfig((*eglDpy), config_Attrib.data(), eglCfg, 1, numConfigs);
        printf("device %i - initialize result %d\n", i, found ? 1 : 0);
    }

    return found;	
}

void print_EGL_info(EGLint major, EGLint  minor)
{
    std::cout << "Generate EGL context with version " << major << "." << minor << std::endl;
    std::cout << glGetString( GL_VERSION ) << std::endl;
    std::cout << glGetString( GL_VENDOR ) << std::endl;
    std::cout << glGetString( GL_RENDERER ) << std::endl;
    std::cout << glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl << std::endl;
}

void run_EGL()
{
    EGLDisplay eglDpy;
	EGLint numConfigs;
    EGLConfig eglCfg;
    EGLContext eglCtx;
    EGLint major, minor;  
    try {
        printf("Initializing EGL display and EGL configuration... \n");  
        // 1. Initialize EGL & 2. Select an appropriate configuration
        if (platform_specific_EGLdisplay(&eglDpy, &numConfigs, &eglCfg, &major, &minor) != EGL_TRUE)
        {
            eglDpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
            errors::assertEGLError("eglGetDisplay");
            eglInitialize(eglDpy, &major, &minor);
            errors::assertEGLError("eglInitialize");
            printf("Done with EGL_DEFAULT_DISPLAY\n\n");
        }
        else
        {
            printf("Done with PLATFORM SPECIFIC DISPLAY\n\n");
        }

        // PFNEGLCREATESTREAMKHRPROC eglCreateStreamKHR = (PFNEGLCREATESTREAMKHRPROC)eglGetProcAddress("eglCreateStreamKHR");
        // printf("Creating EGLStrem... ");  
        // EGLStreamKHR eglStream = eglCreateStreamKHR(eglDpy, nullptr);
        // if (eglStream == EGL_NO_STREAM_KHR) {
        //     errors::assertEGLError("eglCreateStreamKHR");
        // }
        // printf("Done.\n");

        eglChooseConfig(eglDpy, config_Attrib.data(), &eglCfg, 1, &numConfigs);
        errors::assertEGLError("eglChooseConfig");

        // 3. Bind the API
        eglBindAPI(EGL_OPENGL_API);
        errors::assertEGLError("eglBindAPI");

        // 3. Create a context and make it current
        eglCtx = eglCreateContext(eglDpy, eglCfg, EGL_NO_CONTEXT, context_attrib.data());
        errors::assertEGLError("eglCreateContext");

        // PFNEGLCREATESTREAMPRODUCERSURFACEKHRPROC eglCreateStreamProducerSurfaceKHR = (PFNEGLCREATESTREAMPRODUCERSURFACEKHRPROC)eglGetProcAddress("eglCreateStreamProducerSurfaceKHR");
        // EGLSurface surface = eglCreateStreamProducerSurfaceKHR(eglDpy, eglCfg, eglStream, nullptr);

        eglMakeCurrent(eglDpy, EGL_NO_SURFACE, EGL_NO_SURFACE, eglCtx);
        errors::assertEGLError("eglMakeCurrent");
    } catch(const std::exception& e) {
        printf("ERROR %s\n\n TERMINATE\n", e.what());
        terminate(&eglDpy, &eglCtx);
        return 0;
    }
    
    print_EGL_info(major, minor);

    // from now on use your OpenGL context
    Render * render = new Render();
    int i = 0;
    while(i<10) {
        render->display();
        i++;
    }

    terminate(&eglDpy, &eglCtx);
}

int main(int argc, char *argv[])
{
    run_EGL();
    return 1;
}