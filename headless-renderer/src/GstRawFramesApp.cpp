#include "GstRawFramesApp.h"

#include <sstream>
#ifdef G_OS_UNIX
#include <glib-unix.h>
#endif

#include <ctime>

#include "../utils/FileHandler.h"

Glib::RefPtr<Gst::Buffer> GstRawFramesApp::nextFrameBuffer(double dt) { 
    _render.update(dt);
    const std::vector<uint8_t>& frame = _render.nextFrameAndGetPixels(dt);

    Glib::RefPtr<Gst::Buffer> bufferptr = Gst::Buffer::create(frame.size());

    bufferptr->fill(0, frame.data(), frame.size());

    return bufferptr; 
}

gboolean GstRawFramesApp::pushData(GstRawFramesApp * app) {

    gboolean result = false;
    static uint64_t frame = 0;
    static Glib::RefPtr<Gst::Clock> _clock = Gst::SystemClock::obtain();
    static Gst::ClockTime _baseTime = _clock->get_time();
    static Gst::ClockTime _previousTime = _clock->get_time();
    
    Gst::ClockTime currentTime = _clock->get_time();

    // float elapsedTime = (currentTime - _baseTime)/1000000000.0f; //Gst::ClockTime is measured in nanosecods
    
    // if (elapsedTime >= 1.0f) //one second elapsed
    // {
    //     _baseTime = currentTime;
    //     g_print("FPS count: %ld\n", frame);
    //     frame = 0;
    // }

    if (!app->getStreamApp()->sourceid) {
        return false;
    }
    double dt = (currentTime-_previousTime)/1000000.0; //dt in milliseconds
    auto buffer = app->nextFrameBuffer(dt);
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);

    int fps = 60;
    static guint64 ptimestamp = 0;
    buffer->set_pts(ptimestamp); 
    buffer->set_dts(ptimestamp); 
    buffer->set_duration(gst_util_uint64_scale_int (1, GST_SECOND, fps));
    // gst_println("pts set to: %ld", buffer->get_pts());
    ptimestamp += gst_util_uint64_scale_int (1, GST_SECOND, fps);
    app->_logMetrics.push_back(Constants::Metric{(uint64_t) spec.tv_sec*1000000000 + spec.tv_nsec, ptimestamp});
    
    Gst::FlowReturn ret = app->getStreamApp()->appsrc->push_buffer(buffer); 


    frame++;
    _previousTime = currentTime;

    if (ret != Gst::FlowReturn::FLOW_OK)
    {
        g_print("Error %d\n", (int) ret);
        result = false;
    }
    else {result = true; }

    return result;
}

Gst::PadProbeReturn GstRawFramesApp::onSinkBuffer(const Glib::RefPtr<Gst::Pad>& pad, const Gst::PadProbeInfo& padProbeInfo)
{
    guint64 ptimestamp = padProbeInfo.get_buffer()->get_pts(); 
    // gst_println("pts set to: %ld", ptimestamp);

    return Gst::PadProbeReturn::PAD_PROBE_OK;
}

gboolean GstRawFramesApp::createPipeline(int width, int height) {

    std::stringstream str;
    str << "appsrc name=rawsrc is-live=true caps=video/x-raw,format=RGBA,width=" << width << ",height=" << height << ",framerate=(fraction)60/1 " //
    // "! timestampoverlay "
    // "! queue "
    "! shmsink name=shmsink socket-path=/dev/shm/test wait-for-connection=true shm-size=30000000 ";
    //"! videoconvert ! rawvideoparse width=" << width << " height=" << height << " format=11 ! queue ! video/x-raw, format=RGBA "
    //"! rtpvrawpay chunks-per-frame=2048 ! udpsink name=sink host=127.0.0.1 port=5000";

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

    Glib::RefPtr<Gst::Element> shmsink =_streamApp->pipe->get_element("shmsink");
    if (!shmsink)
        return false;
    
    Gst::Iterator<Gst::Pad> it = shmsink->iterate_pads();
    it.begin();
    Glib::RefPtr<Gst::Pad> pad = *it;
    if (!pad)
        return false;
    
    pad->add_probe(Gst::PadProbeType::PAD_PROBE_TYPE_BUFFER, (Gst::Pad::SlotProbe) sigc::mem_fun(this, &GstRawFramesApp::onSinkBuffer) );

    _streamApp->appsrc = Glib::RefPtr<Gst::AppSrc>::cast_dynamic(_streamApp->pipe->get_element("rawsrc")); 
    _streamApp->appsrc->set_do_timestamp(false);
    _streamApp->appsrc->property_format() = Gst::Format::FORMAT_TIME;
    _streamApp->appsrc->property_emit_signals() = true;
    _streamApp->appsrc->signal_need_data().connect(sigc::bind(sigc::mem_fun(this, &GstRawFramesApp::enableStream), _streamApp));
    _streamApp->appsrc->signal_enough_data().connect(sigc::bind(sigc::mem_fun(this, &GstRawFramesApp::disableStream), _streamApp));

    return true;
}

void GstRawFramesApp::enableStream(guint, RawFramesAppPtr app) {
    
    if (app->sourceid == 0) {
        app->sourceid = g_idle_add_full (G_PRIORITY_HIGH_IDLE+20, (GSourceFunc) &pushData, this, NULL); //call every 1/60 sec to enforce 60fps
    }
}


void GstRawFramesApp::disableStream(RawFramesAppPtr app) {
    if (app->sourceid != 0) {
        g_source_remove(app->sourceid);
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

#ifdef G_OS_UNIX
gboolean
GstRawFramesApp::exit_sighandler (gpointer user_data)
{
  gst_print ("Caught signal, stopping mainloop\n");
  GMainLoop *mainloop = (GMainLoop *) user_data;
  g_main_loop_quit (mainloop);
  return TRUE;
}
#endif

void GstRawFramesApp::run(int argc, char *argv[]) {
    Gst::init(argc, argv);
    gst::log::printGstreamerVersion();

    _mainloopptr = Glib::MainLoop::create();
    _render.init(true); //true tells render that it's running an EGL context
    _logMetrics.reserve(500000);
    if(createPipeline(_render.width, _render.height)) {

        #ifdef G_OS_UNIX
        g_unix_signal_add (SIGINT, GstRawFramesApp::exit_sighandler, _mainloopptr->gobj());
        g_unix_signal_add (SIGTERM, GstRawFramesApp::exit_sighandler, _mainloopptr->gobj());
        #endif
        std::cout << "Setting to PLAYING." << std::endl;
        _streamApp->pipe->set_state(Gst::STATE_PLAYING);

        std::cout << "Running." << std::endl;
        _mainloopptr->run();

            // Clean up nicely:
        std::cout << "Returned. Stopping playback." << std::endl;
        _streamApp->pipe->set_state(Gst::STATE_NULL);
        file_handler::save_vector_as_csv<Constants::Metric>("results", _logMetrics);
    }
    else {
        std::cout << "Couldn't create pipeline." << std::endl;
    }
}