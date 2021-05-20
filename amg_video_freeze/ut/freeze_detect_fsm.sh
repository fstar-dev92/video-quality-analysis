#!/bin/bash
if [ $# -ne 1 ]
then
    echo "USAGE: $0 <folder_name> "
    exit -1
fi
epoch1=`date +%s`
epoch2="_freeze_fsm.csv"
epoch=$epoch1$epoch2

printf "File Name , Start pts , End pts , Duration (in ms)\n" >$epoch
folder_name=$1
for filename in $(find "$folder_name" -type f | grep -E "\.ts$|\.mp4$")
do
    echo filename "$filename"
    echo "ffmpeg -i "$filename" -pix_fmt yuv420p -f rawvideo  -s 1920x1080 - | "$(dirname $0)/../bin/amg_video_freeze_detect_ut_fsm" /dev/stdin 1920 1080 40 0 1 1000 i420 "$epoch" "$filename""
ffmpeg -i "$filename" -pix_fmt yuv420p -f rawvideo  -s 1920x1080 - | "$(dirname $0)/../bin/amg_video_freeze_detect_ut_fsm" /dev/stdin 1920 1080 40 0 1 1000 i420 "$epoch" "$filename"
done

