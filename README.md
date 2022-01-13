# Distributed Cloud Gaming Pipeline

> The sources used for this project work are cataloged in [this Notion notebook](https://psyugengames.notion.site/Project-Work-cda5b4615b99439ea44ce406825f8185).  
> The report is written on [this Overleaf project](https://www.overleaf.com/read/cxsnttvjtshz).

# Structure of the repository
```bash
.
├── gstreamer-live-streaming                        # Live streaming WebRTC server (C)
│   ├── bin/                                            # Folder containing the install script output
│   ├── gstreamer-cheatsheet.md                         # Overview of GStreamer's most important elements
│   ├── install.sh                                      # install script
│   ├── media-live-streamer.c                           # source code
├── openGL-offscreen-rendering/headless-renderer    # Example of headless rendering a 3D revolving cube using EGL (C++)
│   ├── frames/                                         # Folder containing the produced jpeg frames
│   ├── CMakeLists.txt                                  # CMake boilerplate
│   ├── Errors.h                                        # Header with error handling functions
│   ├── main.cpp                                        # entry point; initializes EGL and calls Render class in loop
│   ├── Render.[h|cpp]                                  # Render class to render the 3D cube 
```

# Installing the Media Live Streamer
## Dependencies
The OS used to develop the application is Ubuntu 20.04, for which the default GStreamer library downloaded by apt is the 1.16. For better support with OpenGL, the latest stable version was installed, **1.18**. [To upgrade the repositories](https://askubuntu.com/questions/1377561/is-it-possible-to-upgrade-gstreamer-and-libx264-on-ubuntu-18-04) needed to install gstreamer-1.18 run:
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

## Application
To install the application simply run (from the current folder):
```bash
cd gstreamer-live-streaming
./install.sh
```
which will place the executable file in the folder gstreamer-live-streaming/bin.

To run the application run:
```bash
cd bin
./mls
```
# Installing the Headless Renderer
## Dependencies
The libraries used in this application can be installed running the following cmd:
```bash
sudo apt install mesa-utils mesa-common-dev libglapi-mesa libegl1-mesa-dev libegl-dev libjpeg-turbo8-dev libpng-dev
```
## Application
To install the application run:
```bash
cd openGL-offscreen-rendering/headless-renderer
cmake .
make all
```
then, to run it:
```bash
./main
```
