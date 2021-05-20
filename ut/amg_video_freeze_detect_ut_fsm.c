#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "amg_video_freeze_detect.h"

int main(int argc, char *argv[])
{
    FILE *Fin;
    FILE *Fout;
    
    if (argc < 11)
    {
        printf("Usage: %s <infile> <wo> <ho> <pts_interval_ms> <skip_roi> <skip_line> <threshold_ms> <color_type> <report_file_name> <video_file_name>\n", argv[0]);
        exit(1);
    }

    Fin=fopen(argv[1],"rb");

    if( Fin == NULL)
    {
        printf("\nUnable to open File\n\n");
        return -1;
    }

    Ipp64f pValue1, pValue2;
    int wo = atoi(argv[2]);
    int ho = atoi(argv[3]);
    double pts_interval_ms = atof(argv[4]);
    int skip_roi = atoi(argv[5]);
    int skip_line = atoi(argv[6]);
    int threshold_ms = atoi(argv[7]);
    int color_type = FORMAT_UNDEFINED;
    if(0 == strncmp(argv[8], "i420", strlen("i420")))
    {
        color_type = FORMAT_I420;
    }
    char file[32];
    strcpy(file,argv[9]);
    char video_file[1024]; 
    strcpy(video_file,argv[10]);

    amg_video_freeze_init_params_t init_params;
    memset(&init_params, 0, sizeof(amg_video_freeze_init_params_t));
    init_params.silent = 0;
    init_params.no_freeze_to_freeze_threshold_ms = threshold_ms;
    init_params.freeze_to_no_freeze_threshold_ms = 120;
    init_params.freeze_detect_threshold = 2.2;
    init_params.freeze_detect_threshold_uv = 2.2;
    init_params.freeze_detect_roi.x = skip_roi;
    init_params.freeze_detect_roi.y = skip_roi;
    init_params.freeze_detect_roi.w = wo - skip_roi;
    init_params.freeze_detect_roi.h = ho - skip_roi;
    init_params.skip_line = skip_line;
    init_params.intermediate_freeze_to_no_freeze_frames_threshold = 2;
    init_params.pts_interval_ns=(int64_t)(pts_interval_ms * MS_TO_NS_CONVERSION);
    int count = 0;

    FILE *f1=fopen(file,"a");
    amg_video_freeze_input_params_t input_params;
    amg_video_freeze_output_params_t output_params;
    input_params.width    = wo;
    input_params.height   = ho;
    input_params.in_image = malloc((size_t)(wo*ho*3/2));
    input_params.buffer_pts = 0;
    input_params.color_type = color_type;
    output_params.start_pts = 0;
    output_params.end_pts = 0;
    output_params.duration_ms = 0;

    AmgErrorLogParams freeze_params;

    int freeze_end=0;
    void * freeze_ctx = amg_video_freeze_init(&init_params);
    while(fread(input_params.in_image, 1, (size_t)(ho*wo*3/2), Fin)>0)
    {
        input_params.buffer_pts += (uint64_t)(pts_interval_ms * MS_TO_NS_CONVERSION);
        amg_video_freeze_detect(freeze_ctx, &input_params, &freeze_params, &output_params);
        printf("process pts %lu is_freeze %d for %ld ms\n",input_params.buffer_pts/1000000, freeze_params.status, output_params.duration_ms);
        if(freeze_params.status == ERROR_TRUE)
        {
            freeze_end=1;
            //printf("Freeze duration %lu sec\n", freeze_params.duration_s);
        }
        else if(freeze_end == 1)
        {
            printf("total freeze duration is %ld ms\n",(output_params.end_pts-output_params.start_pts)/1000000);
            fprintf(f1,"%s , %ld , %ld , %ld\n",video_file , output_params.start_pts , (int64_t)output_params.end_pts,(output_params.end_pts-output_params.start_pts)/1000000);
            freeze_end=0;
        }
        if(0)
        //if(is_freeze)
        {
            char filename[1024];
            snprintf(filename, 1024, "img_%d.yuv", count);
            Fout=fopen(filename,"wb");
            if( Fout == NULL)
            {
                printf("\nUnable to open File\n\n");
                return -1;
            }

            fwrite(input_params.in_image, 1, (size_t)(ho*wo*3/2), Fout);
            fclose(Fout);
        }
        count++;
    }
    if(freeze_end == 1)
    {
            printf("total freeze duration is %ld ms\n",((int64_t)input_params.buffer_pts-output_params.start_pts)/1000000);
            fprintf(f1,"%s , %ld , %ld , %ld\n",video_file , (int64_t)output_params.start_pts , (int64_t)input_params.buffer_pts,((int64_t)input_params.buffer_pts-output_params.start_pts)/1000000);
    }
   amg_video_freeze_deinit(freeze_ctx);
    printf("total total frames %d\n", count);
    fclose(Fin);

    return 0;
}
