#ifndef _CONTEXT_EGL_H_
#define _CONTEXT_EGL_H_

#include "../utils/glLogUtils.hpp"

class ContextEGL {

    static const int pbufferWidth = 9;
    static const int pbufferHeight = 9;
    static const EGLint context_attrib[13];
    static const EGLint config_Attrib[13];

private:
    EGLDisplay eglDpy;
    EGLSurface surface;
	EGLint numConfigs;
    EGLConfig eglCfg;
    EGLContext eglCtx;
    EGLint major, minor;  

    EGLBoolean platformSpecificEGLdisplay(EGLDisplay * eglDpy, EGLint * numConfigs, EGLConfig * eglCfg, EGLint * major, EGLint  * minor);


public:
    void init();
    void terminate();
};

#endif