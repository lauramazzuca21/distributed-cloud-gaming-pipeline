#ifndef __GST_RAW_FRAMES_APP_H__
#define __GST_RAW_FRAMES_APP_H__

#include <glibmm/main.h>

#include "utils/gstLogUtils.h"
#include "Render.h"

class GstRawFramesApp {

struct RawFramesPipeline {
    Glib::RefPtr<Gst::AppSrc>   appsrc;
    Glib::RefPtr<Gst::Element>  udpsink;
    Glib::RefPtr<Gst::Pipeline> pipe;
    guint sourceid = 0; 
};

typedef std::shared_ptr<RawFramesPipeline> RawFramesAppPtr;

private:
    Render* _render = new Render();
    RawFramesAppPtr _streamApp;
    Glib::RefPtr<Gst::Clock> _clock;
    Gst::ClockTime _baseTime;
    
    Glib::RefPtr<Gst::Buffer> _bufferptr;
    Glib::RefPtr<Glib::MainLoop> _mainloopptr;

    void createPipeline(int width, int height);
    void enableStream(guint, RawFramesAppPtr app);
    void disableStream(RawFramesAppPtr app);
    gboolean pushData(gpointer data);
public:
    void run(int argc, char *argv[]);
};

#endif
