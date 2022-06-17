#pragma once
#include <gstreamermm.h>
#include <iostream>
#define DEBUG
namespace gst {
    namespace log {

        static void printGstreamerVersion()
        {
        guint major, minor, micro, nano;

        // Read GStreamer version
        Gst::version(major, minor, micro, nano);

        std::string nano_str;

        switch (nano) {
            case 1: nano_str = "(CSV)"; break;
            case 2: nano_str = "(Prerelease)"; break;
        }

        std::cout << "This program is linked against GStreamer "
            << major << "." << minor << "." << micro << "."
            << nano << " " << nano_str << std::endl << std::endl;
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