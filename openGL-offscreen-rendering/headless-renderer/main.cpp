#include "Render.h"

#include <math.h>
#include <stdio.h>
#include <array>



int main(int argc, char *argv[])
{
//     App *app = &s_app;
//     GError *error = NULL;
//     GstBus *bus;
//     GstCaps *caps;
//     GstVideoInfo info;


//     gst_init (&argc, &argv);


//     GST_DEBUG_CATEGORY_INIT (appsrc_pipeline_debug, "appsrc-pipeline", 0,
//       "appsrc pipeline example");

//   /* create a mainloop to get messages and to handle the idle handler that will
// * feed data to appsrc. */
//   app->loop = g_main_loop_new (NULL, TRUE);
//   app->timer = g_timer_new();

//   // Option 1: Display on screen via xvimagesink
//   app->pipeline = gst_parse_launch("appsrc name=mysource ! video/x-raw-rgb,width=640,height=480 ! ffmpegcolorspace ! videoscale method=1 ! xvimagesink", NULL);

//   // Option 2: Encode using Theora and stream through UDP
//   // NOTE: first launch receiver by executing:
//   //       gst-launch udpsrc port=5000 ! theoradec ! ffmpegcolorspace ! xvimagesink
//   //app->pipeline = gst_parse_launch("appsrc name=mysource ! videorate ! ffmpegcolorspace ! videoscale method=1 ! video/x-raw-yuv,width=640,height=480,framerate=\(fraction\)15/1 ! theoraenc bitrate=700 ! udpsink host=127.0.0.1 port=5000", NULL);

//   g_assert (app->pipeline);

//   bus = gst_pipeline_get_bus (GST_PIPELINE (app->pipeline));
//   g_assert(bus);

//   /* add watch for messages */
//   gst_bus_add_watch (bus, (GstBusFunc) bus_message, app);

//   /* get the appsrc */
//     app->appsrc = gst_bin_get_by_name (GST_BIN(app->pipeline), "mysource");
//     g_assert(app->appsrc);
//     g_assert(GST_IS_APP_SRC(app->appsrc));
//     g_signal_connect (app->appsrc, "need-data", G_CALLBACK (start_feed), app);
//     g_signal_connect (app->appsrc, "enough-data", G_CALLBACK (stop_feed), app);

//   /* set the caps on the source */
//   gst_video_info_set_format(&info, GST_VIDEO_FORMAT_RGBA, 1920, 1080);
//   caps = gst_video_info_to_caps(&info);
//   gst_app_src_set_caps(GST_APP_SRC(app->appsrc), caps);


//   /* go to playing and wait in a mainloop. */
//   gst_element_set_state (app->pipeline, GST_STATE_PLAYING);

//   /* this mainloop is stopped when we receive an error or EOS */
//   g_main_loop_run (app->loop);

//   GST_DEBUG ("stopping");

//   gst_element_set_state (app->pipeline, GST_STATE_NULL);

//   gst_object_unref (bus);
//   g_main_loop_unref (app->loop);


    return 0;
}