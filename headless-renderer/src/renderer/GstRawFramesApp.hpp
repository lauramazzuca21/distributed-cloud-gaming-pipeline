#pragma once

#include <glibmm/main.h>

#include "../../utils/gstLogUtils.hpp"
#include "../../graphics/Renderer.hpp"

class GstRawFramesApp {

struct _RawFramesApp {
    Glib::RefPtr<Gst::AppSrc>   appsrc;
    Glib::RefPtr<Gst::Pipeline> pipe;
    guint sourceid = 0; 
};

typedef std::shared_ptr<_RawFramesApp> RawFramesAppPtr;

private:
    Renderer* _render;
    RawFramesAppPtr _streamApp;    
    Glib::RefPtr<Glib::MainLoop> _mainloopptr;

    gboolean createPipeline(int width, int height);
    void enableStream(guint, RawFramesAppPtr app);
    void disableStream(RawFramesAppPtr app);
    gboolean busCallback(const Glib::RefPtr<Gst::Bus>& bus, const Glib::RefPtr<Gst::Message>& message);
    
    static gboolean pushData(GstRawFramesApp * app);
    #ifdef G_OS_UNIX
    static gboolean exit_sighandler (gpointer user_data);
    #endif

public:
    void run(int argc, char *argv[]);
    RawFramesAppPtr getStreamApp() { return _streamApp; }
    Glib::RefPtr<Gst::Buffer> nextFrameBuffer(double dt);

    // GstRawFramesApp();
    ~GstRawFramesApp() {
        // _mainloopptr->unreference();
        // _streamApp->appsrc->unreference();
        // _streamApp->pipe->unreference();
    }

};
