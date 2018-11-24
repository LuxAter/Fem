#!/bin/bash
if [ -z "$3" ]; then
  SPEED="1"
else
  SPEED=$(echo "1/$3" | bc)
fi

ffmpeg -i $1/0000000$1.svg -vf palettegen $1/$(basename $1)_palette.png
ffmpeg -i $1/%07d$1.svg -i $1/$(basename $1)_palette.png -lavfi setpts=$SPEED*PTS,paletteuse $1/$(basename $1).$2
