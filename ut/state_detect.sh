#!/bin/bash
if [ $# -ne 0 ]
then
    echo "USAGE : $0"
    exit -1
fi
$(dirname $0)/amg_video_state_detect_ut  2 2 10 0 2 2 0 0 1 0 1 1 1 1 1 1 1 0 0 0 0
$(dirname $0)/amg_video_state_detect_ut  2 2 10 0 3 3 0 0 1 0 1 1 1 1 1 1 1 0 0 0 0
$(dirname $0)/amg_video_state_detect_ut  2 2 10 0 2 2 0 0 1 0 1 1 0 1 0 1 1 1 0 0 1 
$(dirname $0)/amg_video_state_detect_ut  2 2 10 0 1 1 0 0 1 0 1 1 0 1 0 1 1 1 0 0 1 
$(dirname $0)/amg_video_state_detect_ut  2 2 10 0 0 0 0 0 1 0 1 1 0 1 0 1 1 1 0 0 1 
$(dirname $0)/amg_video_state_detect_ut  2 2 10 0 2 2 0 0 1 0 1 0 0 0 1 1 1 0 1 0 0  
