#!/bin/bash

ffmpeg -i $1/%07d$1.svg $1/$(basename $1).$2
