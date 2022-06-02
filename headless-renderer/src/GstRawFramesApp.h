#ifndef __GST_RAW_FRAMES_APP_H__
#define __GST_RAW_FRAMES_APP_H__

#include <glibmm/main.h>

#include "../utils/gstLogUtils.h"
#include "Render.h"

class GstRawFramesApp {

struct _RawFramesApp {
    Glib::RefPtr<Gst::AppSrc>   appsrc;
    Glib::RefPtr<Gst::Pipeline> pipe;
    guint sourceid = 0; 
};

typedef std::shared_ptr<_RawFramesApp> RawFramesAppPtr;

private:
    Render* _render = new Render(true);
    RawFramesAppPtr _streamApp;    
    Glib::RefPtr<Glib::MainLoop> _mainloopptr;

    /*this function creates the pipeline and bounds the enable/disable Stream methods*/
    gboolean createPipeline(int width, int height);
    /*In the need-data callback, we add our pushData function as an idle handler to the main loop. The glib main loop will call this 
    * function from its main loop. When appsrc emits enough-data signal, we just remove this idle handler so that it is not called anymore. */
    void enableStream(guint, RawFramesAppPtr app);
    void disableStream(RawFramesAppPtr app);
    /*callback to bind to the pipeline bus which receives messages about the status of the pipeline*/
    gboolean busCallback(const Glib::RefPtr<Gst::Bus>& bus, const Glib::RefPtr<Gst::Message>& message);
    /*this is the core of the App, it's the funciton that gets called by the loop to push data on the pipeline through the appsrc*/
    static gboolean pushData(GstRawFramesApp * app);
    #ifdef G_OS_UNIX
    static gboolean exit_sighandler (gpointer user_data);
    #endif

public:
    std::vector<Constants::Metric> _logMetrics;
    
    void run(int argc, char *argv[]);
    /*this getter is needed to retrieve the app pointer when pushData is called with pointer to an instance to this class*/
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
