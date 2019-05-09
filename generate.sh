#!/bin/bash

ffmpeg -i $1/%04d.png -vf palettegen $1/palette.png
ffmpeg -i $1/%04d.png -i $1/palette.png -lavfi paletteuse $1/$(basename $1).$2
