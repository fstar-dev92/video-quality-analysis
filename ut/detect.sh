#!/bin/bash
if [ $# -ne 0 ]
then
    echo "USAGE: $0 "
    exit -1
fi
./freeze_detect_fsm.sh /home
./black_detect.sh /home
