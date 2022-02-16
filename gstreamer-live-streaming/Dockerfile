FROM restreamio/gstreamer:latest-prod

ENV TZ=Europe/Madrid

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
# These commands copy your files into the specified directory in the image
# and set that as the working location
COPY . /usr/src/webRTC-streamer
WORKDIR /usr/src/webRTC-streamer

RUN apt-get clean \
    && apt-get update \
     && apt-get install --no-install-recommends -y locales \
                                                    gcc \
                                                    make \
                                                    cmake \
                                                    build-essential \
                                                    pkg-config \
                                                    libssl-dev \
                                                    libnice-dev \
                                                    libsoup2.4-dev \
                                                    libglib2.0-dev \
                                                    libjson-glib-dev

# This command compiles your app using GCC, adjust for your source code
RUN cmake . && make

# This command runs your application, comment out this line to compile only
CMD ["./main"]

LABEL Name=webrtcstreamer Version=0.0.1
