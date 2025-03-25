#!/bin/bash


for infile in ./*.c; do

    # outfile="${infile%.in}.bak"
    # cp $infile $outfile
    sed -i '' "s/VERSION_007/VERSION_008/" $infile  


done

