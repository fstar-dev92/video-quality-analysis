#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "amg_video_black_detect.h"

int main(int argc, char *argv[])
{
    FILE *Fin;
    FILE *Fout;

    if (argc < 13)
    {
        printf("Usage: %s <infile> <wo> <ho> <pts_interval_ms> <skip_roi> <skip_line> <no_of_slices> <threshold_ms> <algo_type> <color_type> <output_file_name> <video_file_name>\n", argv[0]);
        exit(1);
    }

    Fin=fopen(argv[1],"rb");

    if( Fin == NULL)
    {
        printf("\nUnable to open File\n\n");
        return -1;
    }

    int wo = atoi(argv[2]);
    int ho = atoi(argv[3]);
    double pts_interval_ms = atof(argv[4]);
    int64_t pts_interval_ns = (int64_t)((pts_interval_ms) * MS_TO_NS_CONVERSION);
    int skip_roi = atoi(argv[5]);
    int skip_line = atoi(argv[6]);
    int no_of_slices = atoi(argv[7]);
    int threshold_ms = atoi(argv[8]);
    int color_type = FORMAT_UNDEFINED;
    char file[32];
    char video_file[1024]; 
    int algo_type = atoi(argv[9]);
    if(0 == strncmp(argv[10], "i420", strlen("i420")))
    {
        color_type = FORMAT_I420;
    }
    strcpy(file,argv[11]);
    strcpy(video_file,argv[12]);

    amg_video_black_init_params_t init_params;
    amg_video_black_input_params_t input_params;
    amg_video_black_output_params_t output_params;
    AmgErrorLogParams black_params;

    memset(&init_params, 0, sizeof(amg_video_black_init_params_t));
    init_params.silent = 0;
    init_params.no_black_to_black_threshold_ms = threshold_ms;
    init_params.black_to_no_black_threshold_ms = 120;
    init_params.black_frames_threshold  = 20;
    init_params.black_detect_roi.x = skip_roi;
    init_params.black_detect_roi.y = skip_roi;
    init_params.black_detect_roi.w = wo - 2 * skip_roi;
    init_params.black_detect_roi.h = ho - 2 * skip_roi;
    init_params.skip_line = skip_line;
    init_params.pts_interval_ns=pts_interval_ns;
    init_params.no_of_slices = no_of_slices;
    init_params.algo_type = algo_type;
    int count = 0;

    FILE *f1=fopen(file,"a");

    input_params.width    = wo;
    input_params.height   = ho;
    input_params.in_image = malloc((size_t)(wo*ho*3/2));
    input_params.buffer_pts = 0;
    input_params.color_type = color_type;
    output_params.start_pts = 0;
    output_params.end_pts = 0;
    output_params.duration_ms= 0;

    int black_end=0;
    void * black_ctx = amg_video_black_init(&init_params);
    while(fread(input_params.in_image, 1, (size_t)(ho*wo*3/2), Fin)>0)
    {
        input_params.buffer_pts += (uint64_t)(pts_interval_ns);
        amg_video_black_detect(black_ctx, &input_params, &black_params, &output_params);
        printf("process pts %lu is_black %d for %ld ms\n", input_params.buffer_pts/1000000, black_params.status, output_params.duration_ms);
        if(black_params.status == ERROR_TRUE)
        {
            //printf("Black duration %lu sec\n", black_params.duration_s);
            black_end=1;
        }
        else if(black_end == 1)
        {
            printf("total black duration is %ld ms\n",((int64_t)output_params.end_pts - output_params.start_pts)/1000000);
            fprintf(f1,"%s , %ld , %ld , %ld\n",video_file , (int64_t)output_params.start_pts , (int64_t)output_params.end_pts, ((int64_t)output_params.end_pts - output_params.start_pts)/1000000);
            black_end=0;
        }
        if(0)
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
    if(black_end == 1)
    {
            printf("total black duration is %ld ms\n",((int64_t)input_params.buffer_pts-output_params.start_pts)/1000000);
            fprintf(f1,"%s , %ld , %ld , %ld\n",video_file , (int64_t)output_params.start_pts , (int64_t)input_params.buffer_pts,((int64_t)input_params.buffer_pts-output_params.start_pts)/1000000);
    }
    amg_video_black_deinit(black_ctx);
    free(input_params.in_image);
    printf("total total frames %d\n", count);
    fclose(Fin);
    fclose(f1);

    return 0;
}
