#include <locale.h>
#include <glib.h>
#include <gst/gst.h>
#include <gst/sdp/sdp.h>

#ifdef G_OS_UNIX
#include <glib-unix.h>
#endif

#define GST_USE_UNSTABLE_API
#include <gst/webrtc/webrtc.h>

#include <libsoup/soup.h>
#include <json-glib/json-glib.h>

#include <string.h>
#include <stdio.h>
#include <time.h>

#define RTP_PAYLOAD_TYPE "96"
#define SOUP_HTTP_PORT 57778
#define STUN_SERVER "stun.l.google.com:19302"
#define TURN_SERVER_TCP "turn.anyfirewall.com:443?transport=tcp"
#define TURN_SERVER_UDP "turn01.hubl.in?transport=udp"

#define WIDTH "1280"
#define HEIGHT "720"

typedef struct _ReceiverEntry ReceiverEntry;
typedef struct _Metric Metric;
// typedef struct _WebRTCStats WebRTCStats;

GstPad* gst_element_get_first_src_pad(GstElement* element);
GstPad* gst_element_get_first_sink_pad(GstElement* element);

ReceiverEntry *create_receiver_entry (SoupWebsocketConnection * connection);
void destroy_receiver_entry (gpointer receiver_entry_ptr);

static GstPadProbeReturn in_log_metrics_cb (GstPad * pad, GstPadProbeInfo * info, gpointer user_data);
static GstPadProbeReturn out_log_metrics_cb (GstPad * pad, GstPadProbeInfo * info, gpointer user_data);
static gboolean on_webrtcbin_stat (GQuark field_id, const GValue * value, gpointer unused);
static void on_webrtcbin_get_stats (GstPromise * promise, GstElement * webrtcbin);
static gboolean webrtcbin_get_stats (GstElement * webrtcbin);

void on_offer_created_cb (GstPromise * promise, gpointer user_data);
void on_negotiation_needed_cb (GstElement * webrtcbin, gpointer user_data);
void on_ice_candidate_cb (GstElement * webrtcbin, guint mline_index,
    gchar * candidate, gpointer user_data);

void soup_websocket_message_cb (SoupWebsocketConnection * connection,
    SoupWebsocketDataType data_type, GBytes * message, gpointer user_data);
void soup_websocket_closed_cb (SoupWebsocketConnection * connection,
    gpointer user_data);

void soup_http_handler (SoupServer * soup_server, SoupMessage * message,
    const char *path, GHashTable * query, SoupClientContext * client_context,
    gpointer user_data);
void soup_websocket_handler (G_GNUC_UNUSED SoupServer * server,
    SoupWebsocketConnection * connection, const char *path,
    SoupClientContext * client_context, gpointer user_data);

static gchar *get_string_from_json_object (JsonObject * object);

void print_metrics_to_csv();

struct _ReceiverEntry
{
  SoupWebsocketConnection *connection;

  GstElement *pipeline;
  GstElement *webrtcbin;
};

struct _Metric { 
    u_int64_t timestamp; 
    u_int64_t frame_num; 
};

// struct _WebRTCStats { 
//     char[512] log;
// };

#define MAX_METRICS 500000
Metric in_logMetrics[MAX_METRICS];
// WebRTCStats out_logWebRTCStats[MAX_METRICS];
int in_registered_metrics = 0;
// int out_registered_WebRTCStats = 0;
time_t init_time = -1;

const gchar *html_source = " \n \
<html> \n \
  <head> \n \
    <script type=\"text/javascript\" src=\"https://webrtc.github.io/adapter/adapter-latest.js\"></script> \n \
    <script type=\"text/javascript\"> \n \
      var html5VideoElement; \n \
      var fps; \n \
      var interFrameDelay; \n \
      var websocketConnection; \n \
      var webrtcPeerConnection; \n \
      var webrtcConfiguration; \n \
      var reportError; \n \
 \n \
 \n \
      function onLocalDescription(desc) { \n \
        console.log(\"Local description: \" + JSON.stringify(desc)); \n \
        webrtcPeerConnection.setLocalDescription(desc).then(function() { \n \
          websocketConnection.send(JSON.stringify({ type: \"sdp\", \"data\": webrtcPeerConnection.localDescription })); \n \
        }).catch(reportError); \n \
      } \n \
 \n \
 \n \
      function onIncomingSDP(sdp) { \n \
        console.log(\"Incoming SDP: \" + JSON.stringify(sdp)); \n \
        webrtcPeerConnection.setRemoteDescription(sdp).catch(reportError); \n \
        webrtcPeerConnection.createAnswer().then(onLocalDescription).catch(reportError); \n \
      } \n \
 \n \
 \n \
      function onIncomingICE(ice) { \n \
        var candidate = new RTCIceCandidate(ice); \n \
        console.log(\"Incoming ICE: \" + JSON.stringify(ice)); \n \
        webrtcPeerConnection.addIceCandidate(candidate).catch(reportError); \n \
      } \n \
 \n \
 \n \
      function onAddRemoteStream(event) { \n \
        console.log(\"Received stream \" +  event.streams[0]); \n \
        html5VideoElement.srcObject = event.streams[0]; \n \
        if(!html5VideoElement.srcObject.active) \n \
            console.log(\"stream is not active! \"); \n \
        html5VideoElement.play(); \n \
      } \n \
 \n \
 \n \
      function onIceCandidate(event) { \n \
        if (event.candidate == null) \n \
          return; \n \
 \n \
        console.log(\"Sending ICE candidate out: \" + JSON.stringify(event.candidate)); \n \
        websocketConnection.send(JSON.stringify({ \"type\": \"ice\", \"data\": event.candidate })); \n \
      } \n \
 \n \
 \n \
      function onServerMessage(event) { \n \
        var msg; \n \
 \n \
        try { \n \
          msg = JSON.parse(event.data); \n \
        } catch (e) { \n \
          return; \n \
        } \n \
 \n \
        if (!webrtcPeerConnection) { \n \
          webrtcPeerConnection = new RTCPeerConnection(webrtcConfiguration); \n \
          webrtcPeerConnection.ontrack = onAddRemoteStream; \n \
          webrtcPeerConnection.onicecandidate = onIceCandidate; \n \
        } \n \
 \n \
        switch (msg.type) { \n \
          case \"sdp\": onIncomingSDP(msg.data); break; \n \
          case \"ice\": onIncomingICE(msg.data); break; \n \
          default: break; \n \
        } \n \
      } \n \
 \n \
 \n \
      function playStream( hostname, port, path, configuration, reportErrorCB) { \n \
        var l = window.location;\n \
        var wsHost = (hostname != undefined) ? hostname : l.hostname; \n \
        var wsPort = (port != undefined) ? port : l.port; \n \
        var wsPath = (path != undefined) ? path : \"ws\"; \n \
        if (wsPort) \n\
          wsPort = \":\" + wsPort; \n\
        var wsUrl = \"ws://\" + wsHost + wsPort + \"/\" + wsPath; \n \
 \n \
        html5VideoElement = document.getElementById(\"stream\"); \n \
        fps = document.getElementById(\"framePerSecond\"); \n \
        interFrameDelay = document.getElementById(\"totalInterFrameDelay\"); \n \
        html5VideoElement.onloadeddata = (event) => { \n \
        console.log('Yay! The readyState just increased to  ' + \n \
            'HAVE_CURRENT_DATA or greater for the first time.'); \n \
        }; \n \
        html5VideoElement.onsuspend = (event) => {\n \
        console.log('Data loading has been suspended.');\n \
        };\n \
        html5VideoElement.onstalled = (event) => {\n \
        console.log('Failed to fetch data, but trying.');\n \
        };\n \
        html5VideoElement.onerror = function() {\n \
        console.log(\"Error \" + html5VideoElement.error.code + \"; details: \" + html5VideoElement.error.message);\n \
        }\n \
        webrtcConfiguration = configuration; \n \
        reportError = (reportErrorCB != undefined) ? reportErrorCB : function(text) {}; \n \
        console.log(\"Opening connection with: \" + wsUrl); \n \
        websocketConnection = new WebSocket(wsUrl); \n \
        websocketConnection.addEventListener(\"message\", onServerMessage); \n \
      } \n \
 \n \
\n \
    function showStats(results) {  \n \
        results.forEach(res => {\n \
            if (res.type === 'inbound-rtp'){\n \
                fps.innerHTML += res.framesPerSecond + ',';\n \
                let variance =  (res.totalSquaredInterFrameDelay - res.totalInterFrameDelay^2/ res.framesDecoded)/res.framesDecoded;\n \
                interFrameDelay.innerHTML += variance  + ',';\n \
            } \n \
        });\n \
    }\n \
\n \
    window.setInterval(function() {\n \
        if (webrtcPeerConnection) {\n \
            webrtcPeerConnection.getStats(null).then(showStats, err => console.log(err));}\n \
            else {\n \
            console.log('Not connected yet');\n \
        }\n \
    }, 1000);\n \
\n \
      window.onload = function() { \n \
        var config = { 'iceServers': [{ 'urls': 'stun:" STUN_SERVER "' }, {'urls': 'turn:"TURN_SERVER_TCP"', credential: 'webrtc', username: 'webrtc'}] }; \n\
        playStream( null, null, null, config, function (errmsg) { console.error(errmsg); }); \n \
      }; \n \
 \n \
    </script> \n \
  </head> \n \
 \n \
  <body> \n \
    <div> \n \
      <video id=\"stream\" playsinline=\"true\" autoplay=\"true\" muted=\"muted\">Your browser does not support video</video> \n \
    </div> \n \
    <label id=\"framePerSecond\" ></label> <br> \n \
    <label id=\"totalInterFrameDelay\" ></label> \n \
  </body> \n \
</html> \n \
";

//custom function to retrieve first src pad in element
GstPad* gst_element_get_first_src_pad(GstElement* element)
{
	GstIterator *iter = gst_element_iterate_src_pads(element);
	GValue vPad = G_VALUE_INIT;
	GstPad* ret = NULL;
	if (gst_iterator_next(iter, &vPad) == GST_ITERATOR_OK)
	{
		ret = GST_PAD(g_value_get_object(&vPad));
	}
	gst_iterator_free(iter);
	return ret;
} 

//custom function to retrieve first sink pad in element
GstPad* gst_element_get_first_sink_pad(GstElement* element)
{
	GstIterator *iter = gst_element_iterate_sink_pads(element);
	GValue vPad = G_VALUE_INIT;
	GstPad* ret = NULL;
	if (gst_iterator_next(iter, &vPad) == GST_ITERATOR_OK)
	{
		ret = GST_PAD(g_value_get_object(&vPad));
	}
	gst_iterator_free(iter);
	return ret;
} 

ReceiverEntry *
create_receiver_entry (SoupWebsocketConnection * connection)
{
    GError *error;
    ReceiverEntry *receiver_entry;
    GstWebRTCRTPTransceiver *trans;
    GstPad *shm_pad;
    GArray *transceivers;

    receiver_entry = g_slice_alloc0 (sizeof (ReceiverEntry));
    receiver_entry->connection = connection;

    g_object_ref (G_OBJECT (connection));

    g_signal_connect (G_OBJECT (connection), "message",
        G_CALLBACK (soup_websocket_message_cb), (gpointer) receiver_entry);

    error = NULL;
    receiver_entry->pipeline =
        gst_parse_launch ("webrtcbin name=webrtcbin stun-server=stun://"
        STUN_SERVER " turn-server=turn://webrtc:webrtc@" TURN_SERVER_TCP " "
        //"udpsrc port=5000 caps=\"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)RAW, sampling=(string)RGBA, depth=(string)8, width=(string)" WIDTH ", height=(string)" HEIGHT ", colorimetry=(string)BT601-5, payload=(int)96, ssrc=(uint)1103043224, timestamp-offset=(uint)1948293153, seqnum-offset=(uint)27904\" "
        //"! rtpvrawdepay "
        "shmsrc name=shmsrc socket-path=/dev/shm/test do-timestamp=true "
        "! rawvideoparse width=" WIDTH " height=" HEIGHT " format=11 " 
        //"! rtpjitterbuffer faststart-min-packets=2048 latency=2048 " 
        "! videoconvert ! video/x-raw, format=YV12 ! videorate ! video/x-raw, framerate=30/1 "
        "! x264enc bitrate=1800 key-int-max=150 frame-packing=checkerboard speed-preset=medium tune=zerolatency sliced-threads=true threads=4 "
        "! video/x-h264,profile=constrained-baseline ! queue max-size-time=100000000 ! h264parse "
        "! rtph264pay name=payloader aggregate-mode=zero-latency "
        "! application/x-rtp,media=video,encoding-name=H264,profile-level-id=42c01f,payload="
        RTP_PAYLOAD_TYPE " ! webrtcbin. ", &error);
    if (error != NULL) {
        g_error ("Could not create WebRTC streaming pipeline: %s\n", error->message);
        g_error_free (error);
        destroy_receiver_entry ((gpointer) receiver_entry);
        return NULL;
    }

    /*save webrtcbin element*/
    receiver_entry->webrtcbin = gst_bin_get_by_name (GST_BIN (receiver_entry->pipeline), "webrtcbin");
    g_assert (receiver_entry->webrtcbin != NULL);

    /*retrieve its transceivers...*/
    g_signal_emit_by_name (receiver_entry->webrtcbin, "get-transceivers",
        &transceivers);
    g_assert (transceivers != NULL && transceivers->len > 0);
    /*...and set the first one to SENDONLY*/
    trans = g_array_index (transceivers, GstWebRTCRTPTransceiver *, 0);
    g_object_set (trans, "direction", GST_WEBRTC_RTP_TRANSCEIVER_DIRECTION_SENDONLY, NULL);
    g_array_unref (transceivers);

    /*bind webRTC callbacks to webrtcbin element*/
    g_signal_connect (receiver_entry->webrtcbin, "on-negotiation-needed",
        G_CALLBACK (on_negotiation_needed_cb), (gpointer) receiver_entry);

    g_signal_connect (receiver_entry->webrtcbin, "on-ice-candidate",
        G_CALLBACK (on_ice_candidate_cb), (gpointer) receiver_entry);

    /*retrieve shmsrc element from pipeline...*/
    GstElement* shmsrc = gst_bin_get_by_name (GST_BIN (receiver_entry->pipeline), "shmsrc");
    g_assert (shmsrc != NULL);
    /*...retrieve its src pad (which is an always pad -> always present in the element)...*/
    shm_pad = gst_element_get_first_src_pad(shmsrc);
    g_assert (shm_pad != NULL);
    /*...and bind to it the have_data_cb when the pad receives a buffer*/
    gst_pad_add_probe (shm_pad, GST_PAD_PROBE_TYPE_BUFFER, (GstPadProbeCallback) in_log_metrics_cb, NULL, NULL);
    gst_object_unref (shm_pad);
    gst_object_unref (shmsrc);

    // GstPad *webrtcbin_pad = gst_element_get_first_sink_pad(receiver_entry->webrtcbin);
    // g_assert (webrtcbin_pad != NULL);
    // gst_pad_add_probe (webrtcbin_pad, GST_PAD_PROBE_TYPE_BUFFER, (GstPadProbeCallback) out_log_metrics_cb, (gpointer) receiver_entry, NULL);
    // gst_object_unref (webrtcbin_pad);

    gst_element_set_state (receiver_entry->pipeline, GST_STATE_PLAYING);
    return receiver_entry;
    }

void
destroy_receiver_entry (gpointer receiver_entry_ptr)
{
  ReceiverEntry *receiver_entry = (ReceiverEntry *) receiver_entry_ptr;

  g_assert (receiver_entry != NULL);

  if (receiver_entry->pipeline != NULL) {
    gst_element_set_state (GST_ELEMENT (receiver_entry->pipeline),
        GST_STATE_NULL);

    gst_object_unref (GST_OBJECT (receiver_entry->webrtcbin));
    gst_object_unref (GST_OBJECT (receiver_entry->pipeline));
  }

  if (receiver_entry->connection != NULL)
    g_object_unref (G_OBJECT (receiver_entry->connection));

  g_slice_free1 (sizeof (ReceiverEntry), receiver_entry);
}

static GstPadProbeReturn 
in_log_metrics_cb (GstPad *pad, GstPadProbeInfo *info, gpointer user_data)
{
    GstBuffer *buffer;
    buffer = GST_PAD_PROBE_INFO_BUFFER (info);

    static u_int64_t frame = 0;

    in_logMetrics[in_registered_metrics].frame_num = frame;
    in_logMetrics[in_registered_metrics].timestamp = time(NULL);
    ++in_registered_metrics;
    ++frame;

    return GST_PAD_PROBE_OK;
}

static gboolean on_webrtcbin_stat (GQuark field_id, const GValue * value, gpointer unused)
{
  if (GST_VALUE_HOLDS_STRUCTURE (value)) {
      gst_println("stat: \'%s\': %" GST_PTR_FORMAT, g_quark_to_string (field_id),
        gst_value_get_structure (value));
    // GST_WARNING("stat: \'%s\': %" GST_PTR_FORMAT, g_quark_to_string (field_id),
    //     gst_value_get_structure (value));
  } else {
    GST_FIXME ("unknown field \'%s\' value type: \'%s\'",
        g_quark_to_string (field_id), g_type_name (G_VALUE_TYPE (value)));
  }

  return TRUE;
}


static void on_webrtcbin_get_stats (GstPromise * promise, GstElement * webrtcbin)
{
  const GstStructure *stats;

  g_return_if_fail (gst_promise_wait (promise) == GST_PROMISE_RESULT_REPLIED);

  stats = gst_promise_get_reply (promise);
  gst_structure_foreach (stats, on_webrtcbin_stat, NULL);

  g_timeout_add_seconds (1, (GSourceFunc) webrtcbin_get_stats, webrtcbin);
}

static gboolean
webrtcbin_get_stats (GstElement * webrtcbin)
{
  GstPromise *promise;

  promise = gst_promise_new_with_change_func ((GstPromiseChangeFunc) on_webrtcbin_get_stats, webrtcbin, NULL);

  GST_TRACE ("emitting get-stats on %" GST_PTR_FORMAT, webrtcbin);
  g_signal_emit_by_name (webrtcbin, "get-stats", NULL, promise);
  gst_promise_unref (promise);

  return G_SOURCE_REMOVE;
}


/**WEBRTC CALLBACKS**/

void
on_offer_created_cb (GstPromise * promise, gpointer user_data)
{
  gchar *sdp_string;
  gchar *json_string;
  JsonObject *sdp_json;
  JsonObject *sdp_data_json;
  GstStructure const *reply;
  GstPromise *local_desc_promise;
  GstWebRTCSessionDescription *offer = NULL;
  ReceiverEntry *receiver_entry = (ReceiverEntry *) user_data;

  reply = gst_promise_get_reply (promise);
  gst_structure_get (reply, "offer", GST_TYPE_WEBRTC_SESSION_DESCRIPTION, &offer, NULL);
  gst_promise_unref (promise);

  local_desc_promise = gst_promise_new ();
  g_signal_emit_by_name (receiver_entry->webrtcbin, "set-local-description",
      offer, local_desc_promise);
  gst_promise_interrupt (local_desc_promise);
  gst_promise_unref (local_desc_promise);

  sdp_string = gst_sdp_message_as_text (offer->sdp);
  gst_print ("Negotiation offer created:\n%s\n", sdp_string);

  sdp_json = json_object_new ();
  json_object_set_string_member (sdp_json, "type", "sdp");

  sdp_data_json = json_object_new ();
  json_object_set_string_member (sdp_data_json, "type", "offer");
  json_object_set_string_member (sdp_data_json, "sdp", sdp_string);
  json_object_set_object_member (sdp_json, "data", sdp_data_json);

  json_string = get_string_from_json_object (sdp_json);
  json_object_unref (sdp_json);

  soup_websocket_connection_send_text (receiver_entry->connection, json_string);
  g_free (json_string);
  g_free (sdp_string);

  gst_webrtc_session_description_free (offer);
}


void
on_negotiation_needed_cb (GstElement * webrtcbin, gpointer user_data)
{
  GstPromise *promise;
  ReceiverEntry *receiver_entry = (ReceiverEntry *) user_data;

  gst_print ("Creating negotiation offer\n");

  promise = gst_promise_new_with_change_func (on_offer_created_cb,
      (gpointer) receiver_entry, NULL);
  g_signal_emit_by_name (G_OBJECT (webrtcbin), "create-offer", NULL, promise);
}


void
on_ice_candidate_cb (G_GNUC_UNUSED GstElement * webrtcbin, guint mline_index,
    gchar * candidate, gpointer user_data)
{
  JsonObject *ice_json;
  JsonObject *ice_data_json;
  gchar *json_string;
  ReceiverEntry *receiver_entry = (ReceiverEntry *) user_data;

  ice_json = json_object_new ();
  json_object_set_string_member (ice_json, "type", "ice");

  ice_data_json = json_object_new ();
  json_object_set_int_member (ice_data_json, "sdpMLineIndex", mline_index);
  json_object_set_string_member (ice_data_json, "candidate", candidate);
  json_object_set_object_member (ice_json, "data", ice_data_json);

  json_string = get_string_from_json_object (ice_json);
  json_object_unref (ice_json);

  soup_websocket_connection_send_text (receiver_entry->connection, json_string);
    if (json_string != NULL)
        g_free (json_string);
}

/**SOUP SERVER CALLBACKS**/

void
soup_websocket_message_cb (G_GNUC_UNUSED SoupWebsocketConnection * connection,
    SoupWebsocketDataType data_type, GBytes * message, gpointer user_data)
{
  gsize size;
  gchar *data;
  gchar *data_string;
  const gchar *type_string;
  JsonNode *root_json;
  JsonObject *root_json_object;
  JsonObject *data_json_object;
  JsonParser *json_parser = NULL;
  ReceiverEntry *receiver_entry = (ReceiverEntry *) user_data;

  switch (data_type) {
    case SOUP_WEBSOCKET_DATA_BINARY:
      g_error ("Received unknown binary message, ignoring\n");
      g_bytes_unref (message);
      return;

    case SOUP_WEBSOCKET_DATA_TEXT:
      data = g_bytes_unref_to_data (message, &size);
      /* Convert to NULL-terminated string */
      data_string = g_strndup (data, size);
      g_free (data);
      break;

    default:
      g_assert_not_reached ();
  }

  json_parser = json_parser_new ();
  if (!json_parser_load_from_data (json_parser, data_string, -1, NULL))
    goto unknown_message;

  root_json = json_parser_get_root (json_parser);
  if (!JSON_NODE_HOLDS_OBJECT (root_json))
    goto unknown_message;

  root_json_object = json_node_get_object (root_json);

  if (!json_object_has_member (root_json_object, "type")) {
    g_error ("Received message without type field\n");
    goto cleanup;
  }
  type_string = json_object_get_string_member (root_json_object, "type");

  if (!json_object_has_member (root_json_object, "data")) {
    g_error ("Received message without data field\n");
    goto cleanup;
  }
  data_json_object = json_object_get_object_member (root_json_object, "data");

  if (g_strcmp0 (type_string, "sdp") == 0) {
    const gchar *sdp_type_string;
    const gchar *sdp_string;
    GstPromise *promise;
    GstSDPMessage *sdp;
    GstWebRTCSessionDescription *answer;
    int ret;

    if (!json_object_has_member (data_json_object, "type")) {
      g_error ("Received SDP message without type field\n");
      goto cleanup;
    }
    sdp_type_string = json_object_get_string_member (data_json_object, "type");

    if (g_strcmp0 (sdp_type_string, "answer") != 0) {
      g_error ("Expected SDP message type \"answer\", got \"%s\"\n",
          sdp_type_string);
      goto cleanup;
    }

    if (!json_object_has_member (data_json_object, "sdp")) {
      g_error ("Received SDP message without SDP string\n");
      goto cleanup;
    }
    sdp_string = json_object_get_string_member (data_json_object, "sdp");

    gst_print ("Received SDP:\n%s\n", sdp_string);

    ret = gst_sdp_message_new (&sdp);
    g_assert_cmphex (ret, ==, GST_SDP_OK);

    ret = gst_sdp_message_parse_buffer ((guint8 *) sdp_string,
        strlen (sdp_string), sdp);
    if (ret != GST_SDP_OK) {
      g_error ("Could not parse SDP string\n");
      goto cleanup;
    }

    answer = gst_webrtc_session_description_new (GST_WEBRTC_SDP_TYPE_ANSWER,
        sdp);
    g_assert_nonnull (answer);

    promise = gst_promise_new ();
    g_signal_emit_by_name (receiver_entry->webrtcbin, "set-remote-description",
        answer, promise);
    gst_promise_interrupt (promise);
    gst_promise_unref (promise);
    gst_webrtc_session_description_free (answer);
  } else if (g_strcmp0 (type_string, "ice") == 0) {
    guint mline_index;
    const gchar *candidate_string;

    if (!json_object_has_member (data_json_object, "sdpMLineIndex")) {
      g_error ("Received ICE message without mline index\n");
      goto cleanup;
    }
    mline_index =
        json_object_get_int_member (data_json_object, "sdpMLineIndex");

    if (!json_object_has_member (data_json_object, "candidate")) {
      g_error ("Received ICE message without ICE candidate string\n");
      goto cleanup;
    }
    candidate_string = json_object_get_string_member (data_json_object,
        "candidate");

    gst_print ("Received ICE candidate with mline index %u; candidate: %s\n",
        mline_index, candidate_string);

    if (g_strcmp0(candidate_string, "") != 0)
        g_signal_emit_by_name (receiver_entry->webrtcbin, "add-ice-candidate",
            mline_index, candidate_string);
  } else
    goto unknown_message;

cleanup:
  if (json_parser != NULL)
    g_object_unref (G_OBJECT (json_parser));
  if (data_string != NULL)
     g_free (data_string);
  return;

unknown_message:
  g_error ("Unknown message \"%s\", ignoring", data_string);
  goto cleanup;
}


void
soup_websocket_closed_cb (SoupWebsocketConnection * connection,
    gpointer user_data)
{
  GHashTable *receiver_entry_table = (GHashTable *) user_data;
  g_hash_table_remove (receiver_entry_table, connection);
  gst_print ("Closed websocket connection %p\n", (gpointer) connection);
}


void
soup_http_handler (G_GNUC_UNUSED SoupServer * soup_server,
    SoupMessage * message, const char *path, G_GNUC_UNUSED GHashTable * query,
    G_GNUC_UNUSED SoupClientContext * client_context,
    G_GNUC_UNUSED gpointer user_data)
{
  SoupBuffer *soup_buffer;

  if ((g_strcmp0 (path, "/") != 0) && (g_strcmp0 (path, "/index.html") != 0)) {
    soup_message_set_status (message, SOUP_STATUS_NOT_FOUND);
    return;
  }

  soup_buffer =
      soup_buffer_new (SOUP_MEMORY_STATIC, html_source, strlen (html_source));

  soup_message_headers_set_content_type (message->response_headers, "text/html",
      NULL);
  soup_message_body_append_buffer (message->response_body, soup_buffer);
  soup_buffer_free (soup_buffer);

  soup_message_set_status (message, SOUP_STATUS_OK);
}


void
soup_websocket_handler (G_GNUC_UNUSED SoupServer * server,
    SoupWebsocketConnection * connection, G_GNUC_UNUSED const char *path,
    G_GNUC_UNUSED SoupClientContext * client_context, gpointer user_data)
{
    ReceiverEntry *receiver_entry;
    GHashTable *receiver_entry_table = (GHashTable *) user_data;

    gst_print ("Processing new websocket connection %p\n", (gpointer) connection);

    g_signal_connect (G_OBJECT (connection), "closed",
        G_CALLBACK (soup_websocket_closed_cb), (gpointer) receiver_entry_table);

    receiver_entry = create_receiver_entry (connection);
    g_timeout_add_seconds (1, (GSourceFunc) webrtcbin_get_stats, receiver_entry->webrtcbin);
    g_hash_table_replace (receiver_entry_table, connection, receiver_entry);
}


static gchar *
get_string_from_json_object (JsonObject * object)
{
  JsonNode *root;
  JsonGenerator *generator;
  gchar *text;

  /* Make it the root node */
  root = json_node_init_object (json_node_alloc (), object);
  generator = json_generator_new ();
  json_generator_set_root (generator, root);
  text = json_generator_to_data (generator, NULL);

  /* Release everything */
    if (generator != NULL)
        g_object_unref (generator);
  json_node_free (root);
  return text;
}

#ifdef G_OS_UNIX
gboolean
exit_sighandler (gpointer user_data)
{
  gst_print ("Caught signal, stopping mainloop\n");
  GMainLoop *mainloop = (GMainLoop *) user_data;
  g_main_loop_quit (mainloop);
  return TRUE;
}
#endif

void print_metrics_to_csv()
{
    FILE *in_outfile = fopen("in_results.csv", "w");    
    // FILE *out_outfile = fopen("out_results.csv", "w");    

    if(!in_outfile 
    // || !out_outfile
    )
        return;

    int i = 0;
    for (i = 0; i < in_registered_metrics; i++)
    {
        fprintf(in_outfile,"%ld,%ld\n",in_logMetrics[i].frame_num, in_logMetrics[i].timestamp);
    }

    // for (i = 0; i < out_registered_metrics; i++)
    // {
    //     fprintf(out_outfile,"%ld,%ld\n",out_logMetrics[i].frame_num, out_logMetrics[i].timestamp);
    // }

    fclose(in_outfile);
    // fclose(out_outfile);
    return;
}

int
main (int argc, char *argv[])
{
  GMainLoop *mainloop;
  SoupServer *soup_server;
  GHashTable *receiver_entry_table;

  setlocale (LC_ALL, "");
  gst_init (&argc, &argv);

  receiver_entry_table =
      g_hash_table_new_full (g_direct_hash, g_direct_equal, NULL,
      destroy_receiver_entry);

  mainloop = g_main_loop_new (NULL, FALSE);
  g_assert (mainloop != NULL);

#ifdef G_OS_UNIX
  g_unix_signal_add (SIGINT, exit_sighandler, mainloop);
  g_unix_signal_add (SIGTERM, exit_sighandler, mainloop);
#endif

  soup_server = soup_server_new (SOUP_SERVER_SERVER_HEADER, "webrtc-soup-server", NULL);
  soup_server_add_handler (soup_server, "/", soup_http_handler, NULL, NULL);
  soup_server_add_websocket_handler (soup_server, "/ws", NULL, NULL,
      soup_websocket_handler, (gpointer) receiver_entry_table, NULL);
  soup_server_listen_all (soup_server, SOUP_HTTP_PORT,
      (SoupServerListenOptions) 0, NULL);

  gst_print ("WebRTC page link: http://127.0.0.1:%d/\n", (gint) SOUP_HTTP_PORT);

  g_main_loop_run (mainloop);

  g_object_unref (G_OBJECT (soup_server));
  g_hash_table_destroy (receiver_entry_table);
  g_main_loop_unref (mainloop);

  gst_deinit ();

  print_metrics_to_csv();

  return 0;
}
