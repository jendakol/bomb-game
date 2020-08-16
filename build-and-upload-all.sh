#!/bin/bash

./build-and-upload-frontend.sh && \
  pio run --target upload && \
  pio device monitor
