gcc $(dirname "$0")/amg_video_freeze_detect_ut1.c -g -lippi -lipps -lippcore -lippcc -lippvm -lippcv -lm -I/opt/intel/ipp/include -I./inc -L/opt/intel/ipp/lib/intel64 -o $(dirname "$0")/amg_freeze_detect_ut1
gcc $(dirname "$0")/amg_video_black_detect_ut.c -g -lippi -lipps -lippcore -lippcc -lippvm -lippcv -lm -I/opt/intel/ipp/include -I./inc -L/opt/intel/ipp/lib/intel64 -o $(dirname "$0")/amg_black_detect

