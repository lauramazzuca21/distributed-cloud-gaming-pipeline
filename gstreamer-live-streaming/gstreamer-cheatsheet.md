#GStreamer C & CLI Cheatsheet

##Initialization


##GObject
##GstElement and GstElementFactory
###C
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

###CLI

List all available elements types:
```
gst-inspect-1.0
```
List specific element-s properties:
```
gst-inspect-1.0 <element-name>
```

##Pipeline
###C
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









