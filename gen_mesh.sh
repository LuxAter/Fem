#!/bin/bash

OPTIND=1

QUALITY=
AREA=
OUT=

function show_help() {
  echo "gen_mesh.sh [-a AREA] [-q QUALITY] [-o OUT_FILE] INPUT"
  echo "  -a    Maximum area of a triangular element"
  echo "  -q    Minimum angle of the triangular element in degrees"
  echo "  -o    Output file path"
}

while getopts "h?q:a:o:" opt; do
  case "$opt" in
    h|\?)
      show_help
      exit 0
      ;;
    q)
      QUALITY=$OPTARG
      ;;
    a)
      AREA=$OPTARG
      ;;
    o)
      OUT=$OPTARG
      ;;
  esac
done

shift $((OPTIND-1))
[ "${1:-}" == "--" ] && shift

if [[ ! -z $QUALITY ]] && [[ ! -z $AREA ]]; then
  ./triangle -pznq$QUALITY -a$AREA "$1"
elif [[ ! -z $QUALITY ]]; then
  ./triangle -pznq$QUALITY "$1"
elif [[ ! -z $AREA ]]; then
  ./triangle -pzna$AREA "$1"
else
  ./triangle -pzn "$1"
fi

FILE="${1%.*}.1"

if [[ -z $OUT ]]; then
  OUT="$(basename "$1")"
  OUT="./.mesh/${OUT%.*}"
fi

if [[ ! -d "$OUT" ]]; then
  mkdir -p "$OUT"
fi

mv "$FILE.poly" "$OUT/$(basename $OUT).poly"
mv "$FILE.ele" "$OUT/$(basename $OUT).ele"
mv "$FILE.node" "$OUT/$(basename $OUT).node"
mv "$FILE.neigh" "$OUT/$(basename $OUT).neigh"
