#pragma once

#include <unistd.h>
#include <signal.h>
#include <stdarg.h>
#include <iostream>

namespace logger {
    static int print(const char * format, ...) {
        va_list vl;
        va_start(vl, format);
        auto ret = vprintf(format, vl);
        va_end(vl);
        return ret;
    }

    static void printErr(const char * format, ...) {
        va_list vl;
        va_start(vl, format);
        auto ret = vprintf(format, vl);
        va_end(vl);

		fflush(stdout);
		fflush(stderr);
		kill( getpid(), SIGKILL );
    }
}