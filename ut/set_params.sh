#!/bin/bash
export width, height, pts_interval, pix_fmt
if [ $# -ne 1 ]
then
    echo "USAGE:$0 <filename>"
    exit -1
fi
width=`ffprobe  -select_streams v:0  -show_entries program_stream=width -of default=nw=1:nk=1 $1`
height=`ffprobe  -select_streams v:0  -show_entries program_stream=height -of default=nw=1:nk=1 $1`
pix_fmt=`ffprobe  -select_streams v:0  -show_entries program_stream=pix_fmt -of default=nw=1:nk=1 $1`
IFS="/" read -a arr <<< `ffprobe  -select_streams v:0  -show_entries program_stream=r_frame_rate -of default=nw=1:nk=1 $1`
pts_interval=`printf %.6f "$((10**6 * 1000 * ${arr[1]} / ${arr[0]}))e-6"`
