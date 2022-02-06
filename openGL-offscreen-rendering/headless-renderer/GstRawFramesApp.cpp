#include "GstRawFramesApp.h"


void GstRawFramesApp::createPipeline(int width, int height) {
    Gst::VideoInfo info;
    info.set_format(Gst::VideoFormat::VIDEO_FORMAT_RGBA, width, height);

    _streamApp = std::make_shared<RawFramesPipeline>();
    _streamApp->appsrc = Gst::AppSrc::create("raw_appsrc");
    if (!_streamApp->appsrc) {
        throw std::runtime_error("Can't create AppSrc");
    }

    _streamApp->udpsink = Gst::ElementFactory::create_element("udpsink");
    if (!_streamApp->udpsink) {
        throw std::runtime_error("Can't create udpSink");
    }

    _streamApp->pipe = Gst::Pipeline::create("pipe");
    if (!_streamApp->pipe) {
        throw std::runtime_error("Can't create pipeline");
    }

    // const char host[10] = "localhost";
    // _streamApp->udpsink->set_property("host", host);
    _streamApp->udpsink->set_property("port", 5000);

    _streamApp->appsrc->set_caps(info.to_caps());
    _streamApp->appsrc->set_live(true);
    _streamApp->appsrc->property_emit_signals() = true;
    _streamApp->appsrc->signal_need_data().connect(sigc::bind(sigc::mem_fun(this, &GstRawFramesApp::enableStream), _streamApp));
    _streamApp->appsrc->signal_enough_data().connect(sigc::bind(sigc::mem_fun(this, &GstRawFramesApp::disableStream), _streamApp));

    _streamApp->pipe->add(_streamApp->appsrc)->add(_streamApp->udpsink);
    _streamApp->appsrc->link(_streamApp->udpsink);
}

/*In the need-data callback, we add our pushData function as an idle handler to the main loop. The glib main loop will call this 
* function from its main loop. When appsrc emits enough-data signal, we just remove this idle handler so that it is not called anymore. */

void GstRawFramesApp::enableStream(guint, RawFramesAppPtr app) {
    if (!app->sourceid == 0) {
        app->sourceid = g_idle_add ((GSourceFunc) &GstRawFramesApp::pushData, NULL);
    }
}


void GstRawFramesApp::disableStream(RawFramesAppPtr app) {
    if (app->sourceid != 0) {
        app->sourceid = 0;
    }
}

gboolean GstRawFramesApp::pushData(gpointer data) {
    if (!_streamApp->sourceid) {
        return Gst::FLOW_CUSTOM_ERROR;
    }

    static Gst::ClockTime time;

    const std::vector<uint8_t>& frame = _render->nextFramePixels();

    _bufferptr = Gst::Buffer::create(frame.size());

    _bufferptr->fill(0, frame.data(), frame.size());

    _bufferptr->set_pts(time);

    time++;

    return _streamApp->appsrc->push_buffer(_bufferptr);
}

void GstRawFramesApp::run(int argc, char *argv[]) {
    Gst::init(argc, argv);
    gst::log::printGstreamerVersion();

    _mainloopptr = Glib::MainLoop::create();

    createPipeline(_render->width, _render->height);

    std::cout << "Setting to PLAYING." << std::endl;
    _streamApp->pipe->set_state(Gst::STATE_PLAYING);
    std::cout << "Running." << std::endl;
    _mainloopptr->run();

    // Clean up nicely:
    std::cout << "Returned. Stopping playback." << std::endl;
    _streamApp->pipe->set_state(Gst::STATE_NULL);
}