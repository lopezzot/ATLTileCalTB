#!/bin/sh
# script to replace TBrun_all.C in geant-val output dirs

TBrun_all="$1/analysis/TBrun_all.C"
OUTPUT="$2"


if [ ! -f $TBrun_all ]; then
    echo "TBrun_all.C not found at $TBrun_all"
    echo "Usage: ./replace_ana.sh ATLTileCalTB_dir OUTPUT_dir"
    exit
fi

if [ ! -d $OUTPUT ]; then
    echo "Output dir not found at $OUTPUT"
    echo "Usage: ./replace_ana.sh ATLTileCalTB_dir OUTPUT_dir"
    exit
fi

for file in $OUTPUT/geant4/*/*/ATLTileCalTB/*/TBrun_all.C; do
    cp -v $TBrun_all $file
done
