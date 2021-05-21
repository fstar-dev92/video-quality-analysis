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
    . ./set_params.sh $filename
    if [ $pix_fmt = yuv420p ]
    then
        pixel_format="i420"
    else 
        pixel_format="not_i420"
    fi
    echo "ffmpeg -i "$filename" -pix_fmt "$pix_fmt" -f rawvideo  -s "$width"x"$height" - | $(dirname $0)/../bin/amg_video_freeze_detect_ut_fsm /dev/stdin "$width" "$height" "$pts_interval" 0 1 1000 "$pixel_format" "$epoch" "$filename""
    ffmpeg -i "$filename" -pix_fmt "$pix_fmt" -f rawvideo  -s "$width"x"$height" - | $(dirname $0)/../bin/amg_video_freeze_detect_ut_fsm /dev/stdin "$width" "$height" "$pts_interval" 0 1 1000 "$pixel_format" "$epoch" "$filename"
done

