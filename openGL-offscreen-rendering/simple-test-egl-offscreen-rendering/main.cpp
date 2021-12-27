#include <GL/glew.h>
#include <GL/glut.h>
#include <EGL/egl.h>

#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <math.h>
#include <stdio.h>

using namespace std;

void CHECK_FRAMEBUFFER_STATUS()
{                                                         
    GLenum status;
    status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER); 
    switch(status) {
    case GL_FRAMEBUFFER_COMPLETE:
        break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
    /* choose different formats */
        break;

    default:
        /* programming error; will fail on all hardware */
        throw "Framebuffer Error";
    }
}

namespace render
{
    int width, height;
    float aspect;

    int const fbo_width = 512;
    int const fbo_height = 512;

    GLuint fb, color, depth;

    void *dumpbuf;
    int dumpbuf_fd;

    float const light_dir[]={1,1,1,0};
    float const light_color[]={1,0.95,0.9,1};

    void init()
    {
        glGenFramebuffers(1, &fb);
        glGenTextures(1, &color);
        glGenRenderbuffers(1, &depth);

        glBindFramebuffer(GL_FRAMEBUFFER, fb);

        glBindTexture(GL_TEXTURE_2D, color);
        glTexImage2D(   GL_TEXTURE_2D, 
                0, 
                GL_RGB8, 
                fbo_width, fbo_height,
                0, 
                GL_RGBA, 
                GL_UNSIGNED_BYTE, 
                NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, fbo_width, fbo_height);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

        GLint red_bits, green_bits, blue_bits, alpha_bits;

        glGetIntegerv(GL_RED_BITS,   &red_bits);
        glGetIntegerv(GL_GREEN_BITS, &green_bits);
        glGetIntegerv(GL_BLUE_BITS,  &blue_bits);
        glGetIntegerv(GL_ALPHA_BITS, &alpha_bits);

        fprintf(stderr, "FBO format R%dG%dB%dA%d\n",
            (int)red_bits,
            (int)green_bits,
            (int)blue_bits,
            (int)alpha_bits );

        CHECK_FRAMEBUFFER_STATUS();

        dumpbuf_fd = open("./fbodump.rgb", O_CREAT|O_SYNC|O_RDWR, S_IRUSR|S_IWUSR);
        assert(-1 != dumpbuf_fd);
        dumpbuf = malloc(fbo_width*fbo_height*3);
        assert(dumpbuf);
    }

    void display()
    {
        static float a=0, b=0, c=0;

        glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_TEXTURE_2D);
        glBindFramebuffer(GL_FRAMEBUFFER, fb);

        glViewport(0,0,fbo_width, fbo_height);

        glClearColor(0,0,0,0);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1, 1, -1, 1, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glBegin(GL_TRIANGLES);
        glColor3f(1,0,0);
        glVertex3f(1,0,0);

        glColor3f(0,1,0);
        glVertex3f(0,1,0);

        glColor3f(0,0,1);
        glVertex3f(0,0,1);
        glEnd();

        glReadBuffer(GL_COLOR_ATTACHMENT0);
       glReadPixels(0,0,fbo_width,fbo_height,GL_RGB,GL_UNSIGNED_BYTE,dumpbuf);
        lseek(dumpbuf_fd, SEEK_SET, 0);
        write(dumpbuf_fd, dumpbuf, fbo_width*fbo_height*3);
    }
};

static const EGLint configAttribs[] = {
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

    EGLint major, minor;    
    eglInitialize(eglDpy, &major, &minor);

    // 2. Select an appropriate configuration
    EGLint numConfigs;
    EGLConfig eglCfg;

    eglChooseConfig(eglDpy, configAttribs, &eglCfg, 1, &numConfigs);

    // 3. Bind the API
    eglBindAPI(EGL_OPENGL_API);

    // 3. Create a context and make it current
    EGLContext eglCtx = eglCreateContext(eglDpy, eglCfg, EGL_NO_CONTEXT, 
    NULL);

    eglMakeCurrent(eglDpy, EGL_NO_SURFACE, EGL_NO_SURFACE, eglCtx);

    glewInit();
    // from now on use your OpenGL context
    render::init();
    render::display();

    // 4. Terminate EGL when finished
    eglTerminate(eglDpy);
    return 0;
}