# GStreamer C & CLI Cheatsheet

## Initialization


## GstObject
```
GObject
    ╰──GInitiallyUnowned
        ╰──GstObject
            ╰──GstAllocator
            ╰──GstBufferPool
            ╰──GstBus
            ╰──GstClock
            ╰──GstControlBinding
            ╰──GstControlSource
            ╰──GstDevice
            ╰──GstDeviceMonitor
            ╰──GstDeviceProvider
            ╰──GstElement
            ╰──GstPad
            ╰──GstPadTemplate
            ╰──GstPlugin
            ╰──GstPluginFeature
            ╰──GstRegistry
            ╰──GstStream
            ╰──GstStreamCollection
            ╰──GstTask
            ╰──GstTaskPool
            ╰──GstTracer
            ╰──GstTracerRecord
```
GstObject gives us basic refcounting, parenting functionality and locking. Most of the functions are just extended for special GStreamer needs and can be found under the same name in the base class of GstObject which is GObject (e.g. g_object_ref becomes gst_object_ref).  

Since GstObject derives from GInitiallyUnowned, it also inherits the floating reference. Be aware that functions such as gst_bin_add and gst_element_add_pad __take ownership of the floating reference__.

## GstElement and GstElementFactory
### C
```
GObject
    ╰──GInitiallyUnowned
        ╰──GstObject
            ╰──GstElement
```
GstElement is the abstract base class needed to construct an element that can be used in a GStreamer pipeline.  
GstElementFactory is the object used to create intances of elements. Since it can be instantiated as an active object, it has both methods and functions.   

GElementFactory is instantiated with the function:  
```
GstElementFactory *
gst_element_factory_find (const gchar * name)
```
This way, we can have a GstElementFactory representing a specific kind of element and use it when we want to instantiate many elements of the same kind.  
The GstElementFactory names possible are defined inside the class GstElementFactory and can be listed through the CLI (see below).
Then, to create an element we use the method:  
```
GstElement *
gst_element_factory_create (GstElementFactory * factory,
                            const gchar * name)
```
passing the previously created GstElementFactory as first parameter and the (optional) name the element should be given.  
Example:  
```
   #include <gst/gst.h>

   GstElement *src;
   GstElementFactory *srcfactory;

   gst_init (&amp;argc, &amp;argv);

   srcfactory = gst_element_factory_find ("filesrc");
   g_return_if_fail (srcfactory != NULL);
   src = gst_element_factory_create (srcfactory, "src");
```

Instead of doing the double passage above we can simply use instead:  
```
GstElement *
gst_element_factory_make (const gchar * factoryname,
                          const gchar * name)
```
used to create a GstElement by providing:  
* the factoryname (same as we would have provided before during GstElementFactory instantiation)
* the optional name of the element itself

Example:  
```
  GstElement *source, *sink;
  source = gst_element_factory_make ("videotestsrc", "source");
  sink = gst_element_factory_make ("autovideosink", "sink");
```

### CLI

List all available elements types:
```
gst-inspect-1.0
```
List specific element-s properties:
```
gst-inspect-1.0 <element-name>
```

## GstState
The possible states an element can be in.
```
 GST_STATE_VOID_PENDING (0) 	//no pending state.
GST_STATE_NULL (1) 		//the NULL state or initial state of an element.
GST_STATE_READY (2)		//the element is ready to go to PAUSED.
GST_STATE_PAUSED (3)		//the element is PAUSED, it is ready to accept and process data.
				//Sink elements however only accept one buffer and then block.
GST_STATE_PLAYING (4)		//the element is PLAYING, the GstClock is running and the data is flowing. 
```
### C
States can be changed using:
```
GstStateChangeReturn
gst_element_set_state (GstElement * element,
                       GstState state)
```
This function will try to set the requested state by going through all the intermediary states and calling the class's state change function for each.  
This function can return GST_STATE_CHANGE_ASYNC, in which case the element will perform the remainder of the state change asynchronously in another thread.   
And then it can be checked using:  
```
GstStateChangeReturn
gst_element_get_state (GstElement * element,
                       GstState * state,
                       GstState * pending,
                       GstClockTime timeout)
```
An application can use gst_element_get_state to wait for the completion of the state change, for the duration of the specified timeout (in GstClockTime) or it can wait for a GST_MESSAGE_ASYNC_DONE or GST_MESSAGE_STATE_CHANGED on the bus.
### GstStateChangeReturn
```
 GST_STATE_CHANGE_FAILURE (0)		//the state change failed --ONLY REAL FAILURE
GST_STATE_CHANGE_SUCCESS (1) 		//the state change succeeded
GST_STATE_CHANGE_ASYNC (2)		//the state change will happen asynchronously
GST_STATE_CHANGE_NO_PREROLL (3)	//the state change succeeded but the element cannot produce data in GST_STATE_PAUSED. 
					//This typically happens with live sources. 
```
## Pipeline
### C
```
GObject
    ╰──GInitiallyUnowned
        ╰──GstObject
            ╰──GstElement
                ╰──GstBin
                    ╰──GstPipeline
```
All elements in GStreamer must typically be contained inside a pipeline before they can be used, because it takes care of some clocking and messaging functions.   
A pipeline is a particular type of bin, which is the element used to contain other elements. Therefore all methods which apply to bins also apply to pipelines.  
We create the pipeline with:  
```
  GstElement *pipeline;
  pipeline = gst_pipeline_new ("test-pipeline");
```
and then build it with previously created source and sink elements:  
```
gst_bin_add_many (GstBin * bin,
                  GstElement * element_1,
                  ... ...)
```
Example:  
```
gst_bin_add_many (GST_BIN (pipeline), source, sink, NULL);
```
if we wanted to add only one at the time we could have used:  
```
gboolean
gst_bin_add (GstBin * bin,
             GstElement * element)
```
Finally, we need to link the elements with each other using:  
```
gboolean
gst_element_link (GstElement * src,
                  GstElement * dest)
```
Example:  
```
  if (gst_element_link (source, sink) != TRUE) { //error in pipeline linking
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (pipeline);
    return -1;
  }
```
The order counts, because links must be established following the data flow (this is, from source elements to sink elements).

## GstMessage
Messages are implemented as a subclass of GstMiniObject with a generic GstStructure as the content. This allows for writing custom messages without requiring an API change while allowing a wide range of different types of messages.  


## GstBus
```
GObject
    ╰──GInitiallyUnowned
        ╰──GstObject
            ╰──GstBus
```
It is the object responsible for delivering to the application the GstMessages generated by the elements, in FIFO order and to the application thread. This last point is important, because the actual streaming of media is done in another thread than the application. __Every GstPipeline has one GstBus__. The pipeline will set its bus into _flushing_ state when changing from READY to NULL.  

A message is posted on the bus with the gst_bus_post method.   
```
gboolean
gst_bus_post (GstBus * bus,
              GstMessage * message)
```

With the gst_bus_peek and gst_bus_pop methods one can look at or retrieve a previously posted message.  
```
GstMessage *
gst_bus_peek (GstBus * bus)

GstMessage *
gst_bus_pop (GstBus * bus)
```
The bus can also be polled __synchronously__ with:
```
GstMessage *
gst_bus_poll (GstBus * bus,
              GstMessageType events,
              GstClockTime timeout)
```
To poll __asynchronously__:
```
guint
gst_bus_add_watch_full (GstBus * bus,
                        gint priority,
                        GstBusFunc func,
                        gpointer user_data,
                        GDestroyNotify notify)
```
There can only be a single bus watch per bus, you must remove it before you can set a new one. To remove a bus watch:
```
gboolean
gst_bus_remove_watch (GstBus * bus)
```
## GstPad
If a container embeds multiple streams (one video and two audio tracks, for example), the demuxer will separate them and expose them through different output ports. In this way, different branches can be created in the pipeline, dealing with different types of data.  
__The ports through which GStreamer elements communicate with each other are called pads__. There exists sink pads, through which data enters an element, and source pads, through which data exits an element. It follows naturally that source elements only contain source pads, sink elements only contain sink pads, and filter elements contain both.  

Pads can be of three different types:
* Sometimes Pads: these kind Pads appear when data starts to flow into an Element that had no pads to begin with, so it learns about them together with the media;
* Always Pads: these Pads are the regular pads which are always available;
* Request Pads: these Pads are created on demand. The classical example is the tee element, which has one sink pad and no initial source pads: they need to be requested and then tee adds them. In this way, an input stream can be replicated any number of times. The disadvantage is that linking elements with Request Pads is not as automatic, as linking Always Pads.

### C
```
GObject
    ╰──GInitiallyUnowned
        ╰──GstObject
            ╰──GstPad
                ╰──GstProxyPad
```

To retrieve the Pad from an instatiated Element we use:
```
GstPad *
gst_element_get_static_pad (GstElement * element,
                            const gchar * name)
```

## GstPadTemplate
Padtemplates describe the possible media types a Pad or an ElementFactory can handle. This allows for both inspection of handled types before loading the element plugin as well as identifying pads on elements that are not yet created (request or sometimes pads).  
Templates are useful to create several similar Pads, and also allow early refusal of connections between elements: If the Capabilities of their Pad Templates do not have a common subset (their intersection is empty), there is no need to negotiate further.
### C
```
GObject
    ╰──GInitiallyUnowned
        ╰──GstObject
            ╰──GstPadTemplate
```
To retrieve the PadTemplates associated to a ElementFactory we use:
```
const GList *
gst_element_factory_get_static_pad_templates (GstElementFactory * factory)
```
## GstCaps
The Capabilities (or Caps, for short) of a Pad, then, specify what kind of information can travel through the Pad. __Pads can support multiple Capabilities__ (for example, a video sink can support video in different types of RGB or YUV formats) and Capabilities can be specified as ranges (for example, an audio sink can support samples rates from 1 to 48000 samples per second). However, __the actual information traveling from Pad to Pad must have only one well-specified type__. Through a process known as negotiation, two linked Pads agree on a common type, and thus the Capabilities of the Pads become fixed (they only have one type and do not contain ranges).  
In order for two elements to be linked together, they must share a common subset of Capabilities (Otherwise they could not possibly understand each other). This is the main goal of Capabilities.  

_Bear in mind that some elements query the underlying hardware for supported formats and offer their Pad Caps accordingly (They usually do this when entering the READY state or higher). Therefore, the shown caps can vary from platform to platform, or even from one execution to the next (even though this case is rare)._  

To get Caps from an instantiated Pad we use:
```
GstCaps *
gst_pad_get_current_caps (GstPad * pad)
```

### CLI
# GNOME
## GSignal
### C
GSignals are a crucial point in GStreamer. They allow you to be notified (by means of a callback) when something interesting has happened. Signals are identified by a name, and each GObject has its own signals.

```
g_signal_connect(instance, detailed_signal, c_handler, data)
```
* instance: the instance to connect to. (e.g. a GstElement, in our case)
* detailed_signal: a string of the form "signal-name::detail". 
* c_handler: the GCallback to connect.
* data: data to pass to c_handler calls.
