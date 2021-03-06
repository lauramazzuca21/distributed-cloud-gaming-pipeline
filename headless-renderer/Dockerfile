FROM nvcr.io/nvidia/cudagl:11.0.3-runtime-ubuntu20.04

ENV TZ=Europe/Madrid

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
# These commands copy your files into the specified directory in the image
# and set that as the working location
COPY . /usr/src/headless-renderer
WORKDIR /usr/src/headless-renderer

RUN apt-get clean \
    && apt-get update \
     && apt-get install --no-install-recommends -y locales \
                                                    gcc \
                                                    make \
                                                    cmake 

                                                    
RUN apt-get install -y libgstreamermm-1.0-dev \
                    gstreamer1.0-plugins-good \
                    mesa-utils \
                    mesa-common-dev \
                    libglvnd-dev \
                    libglapi-mesa \
                    libegl1-mesa-dev \
                    libegl-dev \
                    libglew-dev \
                    libglm-dev \
                    libgbm1 

#repos savoury1/ffmpeg4 and savoury1/multimedia to install gstreamer 1.18 instead of 1.16
RUN apt-get install -y software-properties-common \
    && add-apt-repository ppa:savoury1/ffmpeg4 \ 
    && add-apt-repository ppa:savoury1/multimedia \
    && apt-get update \
    && apt-get dist-upgrade -y && \
    apt-get clean && \
    rm -rf /var/lib/apt


RUN ls -la /dev | grep nvidia  

EXPOSE 5000/udp

# This command compiles your app using GCC, adjust for your source code
RUN cmake . && make

# This command runs your application, comment out this line to compile only
CMD ["GST_DEBUG=GST_TRACER:7 GST_TRACERS=latency(flags=pipeline+element+reported) ./main"]

LABEL Name=headlessrenderer Version=0.0.1
