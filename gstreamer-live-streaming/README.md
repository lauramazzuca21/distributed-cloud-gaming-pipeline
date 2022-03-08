# Media Live Streamer
This application runs a WebRTC server, streaming the forwarded raw bytes from the headless-renderer as a webRTC server.
## Dependencies
> These dependencies have been listed here since the environment used for Docker is GStreamer ready. For other dependencies plese see the [Dockerfile]()
The OS used to develop the application is Ubuntu 20.04, for which the default GStreamer library downloaded by apt is the 1.16. For better support with OpenGL, the latest stable version was installed, **1.18**. [To upgrade the repositories](https://askubuntu.com/questions/1377561/is-it-possible-to-upgrade-gstreamer-and-libx264-on-ubuntu-18-04) needed to install gstreamer-1.18 run:
```bash
sudo add-apt-repository ppa:savoury1/ffmpeg4
sudo add-apt-repository ppa:savoury1/multimedia
sudo apt update
sudo apt dist-upgrade
```
Then, to install the libraries needed to build and run the application locally:
```bash
sudo apt install gstreamer1.0-tools gstreamer1.0-nice gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-plugins-good libgstreamer1.0-dev libglib2.0-dev libgstreamer-plugins-bad1.0-dev libsoup2.4-dev libjson-glib-dev
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
### Local setup
To install the application simply run (from the current folder):
```bash
cd gstreamer-live-streaming
cmake .
make
```
which will place the executable file in the folder gstreamer-live-streaming/bin.

To run the application run:
```bash
./main
```

### Docker container setup
To install from a clone of this repository
```bash
sudo docker build . -t webrtc-streamer:latest
```
To download from [docker hub](https://hub.docker.com/repository/docker/lauramazzuca/webrtc-streamer)
```bash
docker push lauramazzuca/webrtc-streamer:<tagname>
```

Then run it with host network configuration with
```bash
sudo docker run --rm --network host -dit webrtc-streamer:latest
```