# GCC support can be specified at major, minor, or micro version
# (e.g. 8, 8.2 or 8.2.0).
# See https://hub.docker.com/r/library/gcc/ for all supported GCC
# tags from Docker Hub.
# See https://docs.docker.com/samples/library/gcc/ for more on how to use this image
FROM gcc:latest

# These commands copy your files into the specified directory in the image
# and set that as the working location
COPY . /usr/src/headless-renderer
WORKDIR /usr/src/headless-renderer

RUN apt-get update

RUN apt-get install -y  cmake \
                    libgstreamermm-1.0-dev \
                    mesa-utils \
                    mesa-common-dev \
                    libglvnd-dev \
                    libglapi-mesa \
                    libegl1-mesa-dev \
                    libegl-dev \
                    libglew-dev \
                    libglm-dev \
                    libgbm1

# This command compiles your app using GCC, adjust for your source code
RUN cmake . && make

# This command runs your application, comment out this line to compile only
CMD ["./main"]

LABEL Name=headlessrenderer Version=0.0.1
