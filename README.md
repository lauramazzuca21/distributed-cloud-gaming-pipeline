# Distributed Cloud Gaming Pipeline

## Structure of the repository
```bash
.
├── gstreamer-live-streaming                        # Live streaming WebRTC server (C)
│   ├── gstreamer-cheatsheet.md                         # Overview of GStreamer's most important elements
│   ├── CMakeLists.txt                                  # CMake boilerplate
│   ├── media-live-streamer.c                           # source code
│   ├── Dockerfile	                                # Docker image boilerplate
├── openGL-offscreen-rendering/headless-renderer    # Example of 3D mesh headless rendering using EGL (C++)
│   ├── main.cpp                                        # entry point of the program
│   ├── src/                                         	# Folder containing the source code
│   ├── graphics/                                       # Folder containing graphics-related classes
│   ├── utils/		                                # Utilities headers 
│   ├── extern/                                         # Extern libraries
│   ├── CMakeLists.txt                                  # CMake boilerplate
│   ├── cmake-modules/                                  # Folder containing modules to find libraries
│   ├── meshes/                                  	# Folder containing the meshes obj files
│   ├── shaders/                                 	# Folder containing the GLSL shader programs
│   ├── Dockerfile	                                # Docker image boilerplate
```

# Installing the Media Live Streamer
## Dependencies
> These dependencies have been listed here since the environment used for Docker is GStreamer ready. For other dependencies plese see the [Dockerfile](https://github.com/lauramazzuca21/distributed-cloud-gaming-pipeline/blob/master/gstreamer-live-streaming/Dockerfile)
  
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
sudo docker build . -t webrtc-streamer:1
```
To download from [docker hub](https://hub.docker.com/repository/docker/lauramazzuca/webrtc-streamer)
```bash
docker pull lauramazzuca/webrtc-streamer:latest
```

Then run it with host network configuration with
```bash
sudo docker run --rm --network host -dit webrtc-streamer:1
```

# Installing the Headless Renderer
## Dependencies
>For the libraries needed to run locally this application see the [Dockerfile](https://github.com/lauramazzuca21/distributed-cloud-gaming-pipeline/blob/master/headless-renderer/Dockerfile).

The host to run the Docker container onto needs to have a NVIDIA GPU and install the NVIDIA Docker toolkit with the following commands:
```bash
$ distribution=$(. /etc/os-release;echo $ID$VERSION_ID) \
   && curl -s -L https://nvidia.github.io/nvidia-docker/gpgkey | sudo apt-key add - \
   && curl -s -L https://nvidia.github.io/nvidia-docker/$distribution/nvidia-docker.list | sudo tee /etc/apt/sources.list.d/nvidia-docker.list
   
$ sudo apt-get update && sudo apt-get install -y nvidia-container-toolkit

$ sudo service docker stop
$ sudo tee /etc/docker/daemon.json <<EOF
{
    "runtimes": {
        "nvidia": {
            "path": "/usr/bin/nvidia-container-runtime",
            "runtimeArgs": []
        }
    },
		"default-runtime": "nvidia"
}
EOF
$ sudo service docker start
```
## Application
### Local setup
To install the application locally run:
```bash
cd headless-renderer
cmake .
make
```
then, to run it:
```bash
./main
```

### Docker container setup
To install from a clone of this repository
```bash
sudo docker build . -t headless-renderer:1
```
To download from [docker hub](https://hub.docker.com/repository/docker/lauramazzuca/headless-renderer)
```bash
docker pull lauramazzuca/headless-renderer:latest
```

Then run it with host network configuration with
```bash
sudo docker run --rm --network host -dit headless-renderer:1
```
