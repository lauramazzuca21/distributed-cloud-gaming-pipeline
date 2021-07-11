#!/bin/bash

gcc media-live-streamer.c -o ./bin/mls `pkg-config --cflags --libs gstreamer-1.0`