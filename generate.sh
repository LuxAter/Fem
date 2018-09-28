#!/bin/bash

ffmpeg -i $1/%07d$1.png $1/$(basename $1).$2
