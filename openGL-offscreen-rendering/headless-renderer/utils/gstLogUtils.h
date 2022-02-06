#ifndef _GST_LOG_UTILS_H_
#define _GST_LOG_UTILS_H_

#include <gstreamermm.h>
#include <iostream>

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
            << nano << " " << nano_str << std::endl;
        }
    }
}
#endif