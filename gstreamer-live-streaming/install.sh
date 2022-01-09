#!/bin/bash

gcc -O0 -ggdb -Wall -fno-omit-frame-pointer media-live-streamer.c -o ./bin/mls `pkg-config --cflags --libs gstreamer-webrtc-1.0 gstreamer-sdp-1.0 libsoup-2.4 json-glib-1.0`