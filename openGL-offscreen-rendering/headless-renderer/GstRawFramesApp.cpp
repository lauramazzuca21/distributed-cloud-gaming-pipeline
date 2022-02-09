#include "GstRawFramesApp.h"
#include <sstream>
/*
    stream with
    gst-launch-1.0 udpsrc port=5000 caps="application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)RAW, sampling=(string)RGBA, depth=(string)8, width=(string)1920, height=(string)1080, colorimetry=(string)BT601-5, payload=(int)96, ssrc=(uint)1103043224, timestamp-offset=(uint)1948293153, seqnum-offset=(uint)27904" ! rtpvrawdepay ! rawvideoparse width=1920 height=1080 format=11 ! ximagesink
*/

Glib::RefPtr<Gst::Buffer> GstRawFramesApp::nextFrameBuffer() { 
    
    const std::vector<uint8_t>& frame = _render->nextFramePixels();

    Glib::RefPtr<Gst::Buffer> bufferptr = Gst::Buffer::create(frame.size());

    bufferptr->fill(0, frame.data(), frame.size());

    return bufferptr; 
}

gboolean GstRawFramesApp::pushData(GstRawFramesApp * app) {

    gboolean result = true;

    if (!app->getStreamApp()->sourceid) {
        return Gst::FLOW_CUSTOM_ERROR;
    }

    // static Gst::ClockTime time = _clock->get_time() - _baseTime;

    // _bufferptr->set_pts(time);

    Gst::FlowReturn ret = app->getStreamApp()->appsrc->push_buffer(app->nextFrameBuffer());

    if (ret != Gst::FlowReturn::FLOW_OK)
    {
        g_print("Error %d\n", (int) ret);
        result = false;
    }

    return result;
}

gboolean GstRawFramesApp::createPipeline(int width, int height) {

    std::stringstream str;
    str << "appsrc name=rawsrc is-live=true caps=video/x-raw,format=RGBA,width=" << width << ",height=" << height << " ! videoconvert ! rawvideoparse width=" << width << " height=" << height << " format=11 ! queue ! video/x-raw, format=RGBA ! rtpvrawpay ! udpsink name=sink host=127.0.0.1 port=5000";

    _streamApp = std::make_shared<_RawFramesApp>();
    Glib::RefPtr<Gst::Element> pipeline;
    try {
        pipeline = Gst::Parse::launch(str.str());
    } catch(Gst::ParseError e) {
        g_print("ERROR: Can't parse pipeline:\n%s\n", e.what().c_str());
        return false;
    }

    _streamApp->pipe = Glib::RefPtr<Gst::Pipeline>::cast_dynamic(pipeline); 

    Glib::RefPtr<Gst::Bus> bus = _streamApp->pipe->get_bus();
    bus->add_watch((Gst::Bus::SlotMessage) sigc::mem_fun(this, &GstRawFramesApp::busCallback));

    _streamApp->appsrc = Glib::RefPtr<Gst::AppSrc>::cast_dynamic(_streamApp->pipe->get_element("rawsrc")); 
    _streamApp->appsrc->set_do_timestamp(true);
    _streamApp->appsrc->property_emit_signals() = true;
    _streamApp->appsrc->signal_need_data().connect(sigc::bind(sigc::mem_fun(this, &GstRawFramesApp::enableStream), _streamApp));
    _streamApp->appsrc->signal_enough_data().connect(sigc::bind(sigc::mem_fun(this, &GstRawFramesApp::disableStream), _streamApp));

    return true;
}

/*In the need-data callback, we add our pushData function as an idle handler to the main loop. The glib main loop will call this 
* function from its main loop. When appsrc emits enough-data signal, we just remove this idle handler so that it is not called anymore. */

void GstRawFramesApp::enableStream(guint, RawFramesAppPtr app) {
    
    if (app->sourceid == 0) {
        app->sourceid = g_idle_add ((GSourceFunc) &pushData, this);
    }
}


void GstRawFramesApp::disableStream(RawFramesAppPtr app) {
    if (app->sourceid != 0) {
        app->sourceid = 0;
    }
}

gboolean GstRawFramesApp::busCallback(const Glib::RefPtr<Gst::Bus>& bus, const Glib::RefPtr<Gst::Message>& message)
{

    switch(message->get_message_type()){

    case GST_MESSAGE_ERROR: {
        g_print("Error %s\n", message->get_structure().to_string().c_str());
        _mainloopptr->quit();
    }
    break;

    case GST_MESSAGE_EOS:
        g_print("End of stream\n");
        _mainloopptr->quit();
        break;

    default:
        g_print("got message %s\n", message->get_structure().to_string().c_str());
        break;
    }

    return TRUE;
}

void GstRawFramesApp::run(int argc, char *argv[]) {
    Gst::init(argc, argv);
    gst::log::printGstreamerVersion();

    _mainloopptr = Glib::MainLoop::create();

    if(createPipeline(_render->width, _render->height)) {

        std::cout << "Setting to PLAYING." << std::endl;
        _streamApp->pipe->set_state(Gst::STATE_PLAYING);

        std::cout << "Running." << std::endl;
        _mainloopptr->run();

            // Clean up nicely:
        std::cout << "Returned. Stopping playback." << std::endl;
        _streamApp->pipe->set_state(Gst::STATE_NULL);
    }
    else {
        std::cout << "Couldn't create pipeline." << std::endl;
    }

}