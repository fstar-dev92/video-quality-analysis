/** @file amg_video_black_detect_fsm.c
 *  @brief Amagi Black Detector
 *
 *  @author Viswanath Srikanth Bathina (viswanath.b@amagi.com)
 */

#include <unistd.h>
#include <amg_video_black_detect_algo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LOGGER_NAME_LEN 64

typedef struct __amg_video_black_algoContext_t
{
    int             silent;
    int             line_distance_y;
    int             line_offset;
    void            *logger;
    double          black_frames_threshold_y;
} amg_video_black_algoContext_t;

void* amg_video_black_algo_init(amg_video_black_algo_init_params_t *init_params)
{

    amg_video_black_algoContext_t * amg_video_black_algo = (amg_video_black_algoContext_t *) malloc(sizeof(amg_video_black_algoContext_t));
    char logger_name[MAX_LOGGER_NAME_LEN];

    snprintf (logger_name, MAX_LOGGER_NAME_LEN, "com.amagi.amg_video_black_algo");
    amg_video_black_algo->logger = log_init (logger_name);
    amg_video_black_algo->silent = init_params->silent;

    if (init_params->skip_line < 0)
    {
        log_warn(amg_video_black_algo->logger, "Minimum skip_line can be 0");
        amg_video_black_algo->line_distance_y = 1;
    }
    else if (init_params->skip_line > 5)
    {
        log_warn(amg_video_black_algo->logger, "Maximum skip_line can be 5");
        amg_video_black_algo->line_distance_y = 6;
    }
    else
    {
        amg_video_black_algo->line_distance_y = init_params->skip_line + 1;
    }
    amg_video_black_algo->line_offset = 0;
    amg_video_black_algo->black_frames_threshold_y = init_params->black_frames_threshold_y;
    return (void*)amg_video_black_algo;
}
AmgErrorStatus amg_video_black_algo_detect(void *handle, amg_video_black_algo_params_t *params)
{

    amg_video_black_algoContext_t *amg_video_black_algo = (amg_video_black_algoContext_t *)handle;
    Ipp32f denom_black = sqrtf((Ipp32f)(params->roi_black.w * params->roi_black.h / amg_video_black_algo->line_distance_y));

    Ipp64f l2_y;
    Ipp64f rms_black;
    IppiSize roiSize_black  = {params->roi_black.w, params->roi_black.h / amg_video_black_algo->line_distance_y};

    /*rms for black detection is calculated as, rms(t)=l_2(t)/\sqrt{|I(t)|}
     * where I(t) is current image, l_2(t) is L2 norm for I(t), |X| operation gives cardinal value of X*/

    int offset_black    = (params->roi_black.y + amg_video_black_algo->line_offset)  * params->wo + params->roi_black.x;
    
    ippiNorm_L2_8u_C1R(params->in_image + offset_black, params->wo * amg_video_black_algo->line_distance_y, roiSize_black, &l2_y);
    rms_black = l2_y / (denom_black);
    if (!amg_video_black_algo->silent)
    {
        log_info (amg_video_black_algo->logger, "black_rms_y: %lf ", rms_black);
    }
    amg_video_black_algo->line_offset++;
    amg_video_black_algo->line_offset %= amg_video_black_algo->line_distance_y;
    
    if (rms_black < amg_video_black_algo->black_frames_threshold_y)return ERROR_TRUE;
    return ERROR_FALSE;
}

void amg_video_black_algo_deinit(void *handle)
{
    amg_video_black_algoContext_t *amg_video_black_algo = (amg_video_black_algoContext_t *)handle;

    if (amg_video_black_algo)
    {
        log_deinit(amg_video_black_algo->logger);
        free(amg_video_black_algo);
    }
}

