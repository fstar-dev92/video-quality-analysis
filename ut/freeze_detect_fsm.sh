#!/bin/bash
if [ $# -ne 3 ]
then
    echo "USAGE: $0 <input_folder_name> <output_folder_name> <freeze_threshold>"
    exit -1
fi

report_name=$2"/"`date +%s`"_freeze_fsm.csv"
pix_fmt="yuv420p"
pixel_format="i420"
printf "File Name , Start pts , End pts , Duration (in ms)\n" >$report_name
folder_name=$1
for filename in $(find "$folder_name" -type f | grep -E "\.ts$|\.mp4$")
do
    echo filename "$filename"
    . ./$(dirname $0)/set_params.sh $filename
    echo "ffmpeg -i "$filename" -pix_fmt "$pix_fmt" -f rawvideo  -s "$width"x"$height" - | $(dirname $0)/../bin/amg_video_freeze_detect_ut_fsm /dev/stdin "$width" "$height" "$pts_interval" 0 1 $3 "$pixel_format" "$report_name" "$filename""
    ffmpeg -i "$filename" -pix_fmt "$pix_fmt" -f rawvideo  -s "$width"x"$height" - | $(dirname $0)/../bin/amg_video_freeze_detect_ut_fsm /dev/stdin "$width" "$height" "$pts_interval" 0 1 $3 "$pixel_format" "$report_name" "$filename"
done

