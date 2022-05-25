#ifndef __GST_RAW_FRAMES_APP_H__
#define __GST_RAW_FRAMES_APP_H__

#include <glibmm/main.h>

#include "../utils/gstLogUtils.h"
#include "Renderer.h"

class GstRawFramesApp {

struct _RawFramesApp {
    Glib::RefPtr<Gst::AppSrc>   appsrc;
    Glib::RefPtr<Gst::Pipeline> pipe;
    guint sourceid = 0; 
};

typedef std::shared_ptr<_RawFramesApp> RawFramesAppPtr;

private:
    Renderer* _render = new Renderer(true);
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

#endif
