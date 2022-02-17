#ifndef _GL_LOG_UTILS_H_
#define _GL_LOG_UTILS_H_

#include <sstream>
#include <stdexcept>
#include <iostream>
#include <stdarg.h>

#include <EGL/egl.h>
#include <GL/glew.h>
#define EGL_EGLEXT_PROTOTYPES
#include <EGL/eglext.h>

namespace gl {
    namespace log {

        static void printStats() {
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

            GLint imp_fmt, imp_type;

            glGetIntegerv (GL_IMPLEMENTATION_COLOR_READ_FORMAT, &imp_fmt);
            glGetIntegerv (GL_IMPLEMENTATION_COLOR_READ_TYPE,   &imp_type);

            printf ("Supported Color Format/Type: %x/%x\n\n", imp_fmt, imp_type);
        }

        static void print_EGL_info(EGLint major, EGLint  minor)
        {
            std::cout << "Generate EGL context with version " << major << "." << minor << std::endl;
            std::cout << glGetString( GL_VERSION ) << std::endl;
            std::cout << glGetString( GL_VENDOR ) << std::endl;
            std::cout << glGetString( GL_RENDERER ) << std::endl;
            std::cout << glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl << std::endl;
        }

        namespace errors {

            static std::string EGLErrorString(GLenum error)
            {
                switch(error)
                {
                    case EGL_NOT_INITIALIZED:
                        return "EGL_NOT_INITIALIZED";
                    case EGL_BAD_ACCESS:
                        return "EGL_BAD_ACCESS";
                    case EGL_BAD_ALLOC:
                        return "EGL_BAD_ALLOC";
                    case EGL_BAD_ATTRIBUTE:
                        return "EGL_BAD_ATTRIBUTE";
                    case EGL_BAD_CONTEXT:
                        return "EGL_BAD_CONTEXT";
                    case EGL_BAD_CONFIG:
                        return "EGL_BAD_CONFIG";
                    case EGL_BAD_CURRENT_SURFACE:
                        return "EGL_BAD_CURRENT_SURFACE";
                    case EGL_BAD_DISPLAY:
                        return "EGL_BAD_DISPLAY";
                    case EGL_BAD_DEVICE_EXT:
                        return "EGL_BAD_DEVICE_EXT";
                    case EGL_BAD_SURFACE:
                        return "EGL_BAD_SURFACE";
                    case EGL_BAD_MATCH:
                        return "EGL_BAD_MATCH";
                    case EGL_BAD_PARAMETER:
                        return "EGL_BAD_PARAMETER";
                    case EGL_BAD_NATIVE_PIXMAP:
                        return "EGL_BAD_NATIVE_PIXMAP";
                    case EGL_BAD_NATIVE_WINDOW:
                        return "EGL_BAD_NATIVE_WINDOW";
                    case EGL_CONTEXT_LOST:
                        return "EGL_CONTEXT_LOST";
                    case EGL_BAD_STREAM_KHR:
                        return "EGL_BAD_STREAM_KHR";
                    case EGL_BAD_STATE_KHR:
                    return "EGL_BAD_STATE_KHR";
                    default:
                    return "no error";
                }
            }

            static void assertOpenGLError(const std::string& msg) {
                GLenum error = glGetError();

                if (error != GL_NO_ERROR) {
                    std::stringstream s;
                    s << "OpenGL error 0x" << std::hex << error << " at " << msg;
                    throw std::runtime_error(s.str());
                }
            }

            static void assertEGLError(const std::string& msg) {
                EGLint error = eglGetError();

                if (error != EGL_SUCCESS) {
                    std::stringstream s;
                    s << "EGL error " << EGLErrorString(error) << " at " << msg;
                    throw std::runtime_error(s.str());
                }
            }

            static void checkFramebufferStatus()
            {                                                         
                GLenum status;
                status = glCheckFramebufferStatus(GL_FRAMEBUFFER); 
                switch(status) {
                case GL_FRAMEBUFFER_COMPLETE:
                    break;
                case GL_FRAMEBUFFER_UNSUPPORTED:
                    throw std::runtime_error("GL_FRAMEBUFFER_UNSUPPORTED");
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                    throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                    throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
                case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                    throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
                case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                    throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
                default:
                    /* programming error; will fail on all hardware */
                    throw std::runtime_error("Framebuffer Error");
                }
            }

        }
        namespace debug {
            static int print(const char * format, ...) {
                #ifdef DEBUG
                    va_list vl;
                    va_start(vl, format);
                    auto ret = vprintf(format, vl);
                    va_end(vl);
                    return ret;
                #else
                    return 0;
                #endif
            }


        }
    }

}

#endif		//_LOG_UTILS_H_