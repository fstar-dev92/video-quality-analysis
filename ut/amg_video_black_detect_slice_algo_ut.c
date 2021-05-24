#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "amg_video_black_detect_slice_algo.h"

int main(int argc, char *argv[])
{
    FILE *Fin;
    FILE *Fout;

    if (argc < 9)
    {
        printf("Usage: %s <infile> <wo> <ho> <skip_roi> <skip_line> <no_of_slices> <report_name> <video_file_name>\n", argv[0]);
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
    int skip_roi = atoi(argv[4]);
    int skip_line = atoi(argv[5]);
    int no_of_slices=atoi(argv[6]);
    char video_file[1024]; 
    char file[64];
    strcpy(file,argv[7]);
    strcpy(video_file,argv[8]);
    FILE *f1=fopen(file,"a");

    AmgErrorLogParams black_params;

    amg_video_black_slice_algo_init_params_t algo_init_params;
    //algo_init_params.silent = 0;
    algo_init_params.skip_line = skip_line;
    algo_init_params.black_frames_threshold_y = 20;
    algo_init_params.no_of_slices = no_of_slices;
    void *algo_handle = amg_video_black_slice_algo_init(&algo_init_params);

    uint8_t *in_image=malloc((size_t)(wo*ho*3/2));
    amg_video_roi_t black_detect_roi;
    black_detect_roi.x = skip_roi;
    black_detect_roi.y = skip_roi;
    black_detect_roi.w = wo - 2 * skip_roi;
    black_detect_roi.h = ho - 2 * skip_roi;
    fread(in_image, 1, (size_t)(ho*wo*3/2), Fin);
    amg_video_black_algo_params_t algo_params;
    algo_params.in_image=in_image;
    algo_params.roi_black = black_detect_roi;
    algo_params.wo = wo;
    AmgErrorStatus output = amg_video_black_slice_algo_detect(algo_handle,&algo_params);
    printf("file_name: %s, is_black %d\n", video_file, output);
    fprintf(f1, " %s, %d\n", video_file, output);
    
    amg_video_black_slice_algo_deinit(algo_handle);
    fclose(Fin);
    fclose(f1);

    return 0;
}
