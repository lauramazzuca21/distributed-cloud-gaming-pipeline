FROM restreamio/gstreamer:1.18.5.0-prod
ENV TZ=Europe/Madrid

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get clean \
    && apt-get update \
     && apt-get install --no-install-recommends -y locales 

# These commands copy your files into the specified directory in the image
# and set the directory as the working location
COPY . /usr/src/webRTC-streamer
WORKDIR /usr/src/webRTC-streamer

RUN apt-get install -y gcc \
                        make \
                        cmake \
                        # iproute2 \
                        build-essential \
                        pkg-config \
                        libssl-dev \
                        libnice-dev \
                        libsoup2.4-dev \
                        libglib2.0-dev \
                        libjson-glib-dev \
                        libdrm-dev \
                        libva-wayland2

RUN apt-get clean && \
    rm -rf /var/lib/apt
# This command compiles your app using GCC, adjust for your source code
RUN cmake . && make

# This command runs your application, comment out this line to compile only
CMD ["./main"]

LABEL Name=webrtcstreamer Version=0.0.1
