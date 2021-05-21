#!/bin/bash
if [ $# -ne 1 ]
then
    echo "USAGE: $0 <folder_name> "
    exit -1
fi
epoch1=`date +%s`
epoch2="_black_slice_frame.csv"
epoch3="_black_slice_frame_roi.csv"
epoch=$epoch1$epoch2
folder_name=$1
printf "File Name , is_black\n" >$epoch 
for filename in $(find $folder_name -type f | grep -E "\.png$|\.jpg$")
do
    echo filename $filename
    . ./set_params.sh $filename
    echo "ffmpeg -i "$filename" -pix_fmt "$pix_fmt" -f rawvideo  -s "$width"x"$height" - | $(dirname $0)/../bin/amg_video_black_detect_ut_fsm /dev/stdin "$width" "$height" "$frame_rate" 0 1 16 "$epoch" "$filename""
    ffmpeg -i "$filename" -pix_fmt "$pix_fmt" -f rawvideo  -s "$width"x"$height" - | $(dirname $0)/../bin/amg_video_black_detect_ut_fsm /dev/stdin "$width" "$height" "$frame_rate" 0 1 16 "$epoch" "$filename"
done

epoch=$epoch1$epoch3
folder_name=$1
printf "File Name , is_black\n" >$epoch 
for filename in $(find $folder_name -type f | grep -E "\.png$|\.jpg$")
do
    echo filename $filename
    . ./set_params.sh $filename
    echo "ffmpeg -i "$filename" -pix_fmt "$pix_fmt" -f rawvideo  -s "$width"x"$height" - | $(dirname $0)/../bin/amg_video_black_detect_ut_fsm /dev/stdin "$width" "$height" "$frame_rate" 40 1 16 "$epoch" "$filename""
    ffmpeg -i "$filename" -pix_fmt "$pix_fmt" -f rawvideo  -s "$width"x"$height" - | $(dirname $0)/../bin/amg_video_black_detect_ut_fsm /dev/stdin "$width" "$height" "$frame_rate" 40 1 16 "$epoch" "$filename"
done
