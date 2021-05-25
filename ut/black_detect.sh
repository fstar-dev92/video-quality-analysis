#!/bin/bash
if [ $# -ne 3 ]
then
    echo "USAGE: $0 <input_folder_name> <output_folder_name> <freeze_threshold>"
    exit -1
fi

report_name=$2"/"`date +%s`"_black.csv"

pix_fmt="yuv420p"
folder_name=$1
printf "File Name , Start pts , End pts , Duration(in ms)\n" >$report_name
for filename in $(find "$folder_name" -type f | grep -E "\.ts$|\.mp4$")
do
    echo filename "$filename"
    echo report_name $report_name
    . ./$(dirname $0)/set_params.sh $filename
    echo "ffmpeg -i "$filename" -pix_fmt "$pix_fmt" -f rawvideo  -s "$width"x"$height" - | $(dirname $0)/../bin/amg_video_black_detect_ut_fsm /dev/stdin "$width" "$height" "$pts_interval" 0 1 16 $3 1 "$pixel_format" "$report_name" "$filename""
    ffmpeg -i "$filename" -pix_fmt "$pix_fmt" -f rawvideo  -s "$width"x"$height" - | $(dirname $0)/../bin/amg_video_black_detect_ut_fsm /dev/stdin "$width" "$height" "$pts_interval" 0 1 16 $3 1 "$pixel_format" "$report_name" "$filename"
done

