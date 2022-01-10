# Media Live Streamer
This application runs a WebRTC server, streaming an OpenGL loop of frames.
It leverages the _good_  and _nice_ plugins:
* ```webrtcbin```
* ```gstOpenGL```
* ```rtp```
* ``` x264enc```

## Installing dependencies
The OS used is Ubuntu 20.04, for which the default GStreamer library downloaded by apt is the 1.16. For better support with OpenGL, the latest stable version was installed, **1.18**. [To upgrade the repositories](https://askubuntu.com/questions/1377561/is-it-possible-to-upgrade-gstreamer-and-libx264-on-ubuntu-18-04) needed to install gstreamer-1.18 run:
```bash
sudo add-apt-repository ppa:savoury1/ffmpeg4
sudo add-apt-repository ppa:savoury1/multimedia
sudo apt update
sudo apt dist-upgrade
```
Then, to install the libraries needed to build and run the application:
```bash
sudo apt install gstreamer1.0-tools gstreamer1.0-nice gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-plugins-good libgstreamer1.0-dev libglib2.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-gl libsoup2.4-dev libjson-glib-dev
```
### NB
If you already have **anaconda** installed, you already have GStreamer installed. To check run:
```bash
which gst-launch-1.0
```
If you do have it, it will probably be an older version so it would be better to install it outside of anaconda and run:
```bash
conda deactivate
```
if the terminal has conda environment active.

## Installing the Application
To install the application simply run:
```bash
./install.sh
```
which will place the executable file in the folder gstreamer-live-streaming/bin.

To run the application run:
```bash
cd bin
./mls