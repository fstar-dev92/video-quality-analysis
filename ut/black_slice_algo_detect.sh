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
    echo "ffmpeg -i $filename -pix_fmt yuv420p -f rawvideo  -s 1280x720 - | $(dirname $0)/../bin/amg_video_black_detect_slice_algo_ut /dev/stdin 1280 720 40 0 1 16 $epoch $filename"
    ffmpeg -i $filename -pix_fmt yuv420p -f rawvideo  -s 1280x720  - | $(dirname $0)/../bin/amg_video_black_detect_slice_algo_ut /dev/stdin 1280 720 40 0 1 16 $epoch $filename 
done

epoch=$epoch1$epoch3
folder_name=$1
printf "File Name , is_black\n" >$epoch 
for filename in $(find $folder_name -type f | grep -E "\.png$|\.jpg$")
do
    echo filename $filename
    echo "ffmpeg -i $filename -pix_fmt yuv420p -f rawvideo  -s 1280x720 - | $(dirname $0)/../bin/amg_video_black_detect_slice_algo_ut /dev/stdin 1280 720 40 40 1 16 $epoch $filename"
    ffmpeg -i $filename -pix_fmt yuv420p -f rawvideo  -s 1280x720  - | $(dirname $0)/../bin/amg_video_black_detect_slice_algo_ut /dev/stdin 1280 720 40 40 1 16 $epoch $filename 
done
