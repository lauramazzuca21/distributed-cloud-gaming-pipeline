# Media Live Streamer

This very simple application streams a remote live video, itself streaming in RTSP.  
It leverages the _good_ plugins:
* ```rtspsrc```
* ```rtph264depay```
* ```avdec_h264```

## Setup
Install gstreamer-1.16.2 (Ubuntu 20.04 - more recent versions install v. 1.18.2) and its libraries:
```
apt install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio
```
To install the application simply run:
```
./install.sh
```
which will place the executable file in the folder gstreamer-live-streaming/bin.

### NB
If you already have anaconda installed, you already have GStreamer installed. To check run:
```
which gst-launch-1.0
```
## Run
to run the application run:
```
cd bin
./mls
```
# TODO
- [ ] add audio queue
- [ ] make the source frames locally stored instead of a remote stream
