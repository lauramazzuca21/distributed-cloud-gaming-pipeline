#include "GstRawFramesApp.hpp"
#include <sstream>
#ifdef G_OS_UNIX
#include <glib-unix.h>
#endif

Glib::RefPtr<Gst::Buffer> GstRawFramesApp::nextFrameBuffer(double dt) { 
    // _render->update(dt);
    const std::vector<uint8_t>& frame = _render->nextFrameBuffer(dt);

    Glib::RefPtr<Gst::Buffer> bufferptr = Gst::Buffer::create(0);

    bufferptr->fill(0, frame.data(), frame.size());

    return bufferptr; 
}

gboolean GstRawFramesApp::pushData(GstRawFramesApp * app) {

    gboolean result = false;
    static int frames = 0;
    static Glib::RefPtr<Gst::Clock> _clock = Gst::SystemClock::obtain();
    static Gst::ClockTime _baseTime = _clock->get_time();
    static Gst::ClockTime _previousTime = _clock->get_time();
    
    Gst::ClockTime currentTime = _clock->get_time();

    // float time = (currentTime - _baseTime)/1000000000.0f; //Gst::ClockTime is measured in nanosecods
    
    // if (time >= 1.0f) //one second elapsed
    // {
    //     _baseTime = currentTime;
    //     // g_print("FPS count: %d\n", frames);
    //     frames = 0;
    // }

    if (!app->getStreamApp()->sourceid) {
        return false;
    }
    double dt = (currentTime-_previousTime)/1000000.0; //dt in milliseconds
    _previousTime = currentTime;
    Gst::FlowReturn ret = app->getStreamApp()->appsrc->push_buffer(app->nextFrameBuffer(dt)); 
    frames++;

    if (ret != Gst::FlowReturn::FLOW_OK)
    {
        g_print("Error %d\n", (int) ret);
        result = false;
    }
    else {result = true; }

    return result;
}

gboolean GstRawFramesApp::createPipeline(int width, int height) {

    std::stringstream str;
    str << "appsrc name=rawsrc is-live=true caps=video/x-raw,format=RGBA,width=" << width << ",height=" << height << ",framerate=60/1 " 
    //"! shmsink socket-path=/dev/shm/test shm-size=2000000";
    "! videoconvert ! rawvideoparse width=" << width << " height=" << height << " format=11 ! queue ! video/x-raw, format=RGBA "
    "! rtpvrawpay chunks-per-frame=2048 ! udpsink name=sink host=127.0.0.1 port=5000";

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

    case GST_MESSAGE_ERROR: 
        g_print("Error %s\n", message->get_structure().to_string().c_str());
        _mainloopptr->quit();
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
    
    _render = new Renderer(true);
    
    Gst::init(argc, argv);
    gst::log::printGstreamerVersion();

    _mainloopptr = Glib::MainLoop::create();

    if(createPipeline(_render->width, _render->height)) {

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
    }
    else {
        std::cout << "Couldn't create pipeline." << std::endl;
    }

}