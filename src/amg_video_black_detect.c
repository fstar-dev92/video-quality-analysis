/** @file amg_video_black_detect_fsm.c
 *  @brief Amagi Black Detector
 *
 *  @author Viswanath Srikanth Bathina (viswanath.b@amagi.com)
 */

#include <unistd.h>
#include "amg_video_black_detect.h"
#include <amg_video_black_detect_fsm.h>
#include <amg_video_black_detect_algo.h>
#include <amg_video_black_detect_slice_algo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LOGGER_NAME_LEN 64
typedef void (*fn_algo_deinit) (void* handle);
typedef int  (*fn_algo_detect) (void *handle, amg_video_black_algo_params_t *params);

typedef struct __amg_video_black_context_t
{
    int                     silent;
    void                    *logger;
    amg_video_roi_t         roi_black;
    void                    *fsm_handle;
    void                    *algo_handle;
    fn_algo_detect          detect;
    fn_algo_deinit          deinit;
} amg_video_black_context_t;

void* amg_video_black_init(amg_video_black_init_params_t *init_params)
{
    amg_video_black_context_t * amg_video_black = (amg_video_black_context_t *) malloc(sizeof(amg_video_black_context_t));
    char logger_name[MAX_LOGGER_NAME_LEN];

    snprintf (logger_name, MAX_LOGGER_NAME_LEN, "com.amagi.amg_video_blackDetector");
    amg_video_black->logger = log_init (logger_name);
    amg_video_black->silent = init_params->silent;

    memcpy(&amg_video_black->roi_black, &init_params->black_detect_roi, sizeof(amg_video_roi_t));
    
    amg_video_black_slice_algo_init_params_t slice_algo_init_params;
    amg_video_black_algo_init_params_t algo_init_params;
    switch (init_params->algo_type)
    {
        case AMG_VIDEO_BLACK_ALGO_TYPE:
            algo_init_params.silent = init_params->silent;
            algo_init_params.skip_line = init_params->skip_line;
            algo_init_params.black_frames_threshold_y = init_params->black_frames_threshold;
            amg_video_black->algo_handle = amg_video_black_algo_init(&algo_init_params);
            amg_video_black->detect = amg_video_black_algo_detect;
            amg_video_black->deinit = amg_video_black_algo_deinit;
            break;
        case AMG_VIDEO_BLACK_SLICE_ALGO_TYPE:
            slice_algo_init_params.silent = init_params->silent;
            slice_algo_init_params.skip_line = init_params->skip_line;
            slice_algo_init_params.black_frames_threshold_y = init_params->black_frames_threshold;
            slice_algo_init_params.no_of_slices=init_params->no_of_slices;
            amg_video_black->detect = amg_video_black_slice_algo_detect;
            amg_video_black->deinit = amg_video_black_slice_algo_deinit;
            amg_video_black->algo_handle = amg_video_black_slice_algo_init(&slice_algo_init_params);
            break;
        default:
            amg_video_black_deinit((void*)amg_video_black);
            return NULL;
    }
    amg_video_black_fsm_init_params_t fsm_init_params;
    fsm_init_params.black_to_no_black_threshold_ms = init_params->black_to_no_black_threshold_ms;
    fsm_init_params.no_black_to_black_threshold_ms = init_params->no_black_to_black_threshold_ms;
    fsm_init_params.pts_interval_ns = init_params->pts_interval_ns;
    fsm_init_params.silent = init_params->silent;
    amg_video_black->fsm_handle = amg_video_black_fsm_init(&fsm_init_params);
    return (void *) amg_video_black;
}

void amg_video_black_deinit(void *handle)
{
    amg_video_black_context_t *amg_video_black = (amg_video_black_context_t *)handle;

    if(amg_video_black)
    {
        amg_video_black_fsm_deinit(amg_video_black->fsm_handle);
        amg_video_black->deinit(amg_video_black->algo_handle);
        log_deinit(amg_video_black->logger);
        free(amg_video_black);
    }
}

static bool verify_roi(amg_video_black_context_t *amg_video_black, int wo, int ho,
        amg_video_roi_t *roi_black)
{
    if (amg_video_black->roi_black.w + amg_video_black->roi_black.x > wo ||
            amg_video_black->roi_black.h + amg_video_black->roi_black.y > ho)
    {
        roi_black->x = roi_black->y = 0;
        roi_black->w = wo;
        roi_black->h = ho;
        return false;
    }
    if (amg_video_black->roi_black.w == 0 || amg_video_black->roi_black.h == 0)
    {
        roi_black->x = 0;
        roi_black->y = 0;
        roi_black->w = wo;
        roi_black->h = ho;
    }
    else
    {
        memcpy(roi_black, &amg_video_black->roi_black, sizeof(amg_video_roi_t));
    }
    return true;
}

int amg_video_black_detect(void * handle, amg_video_black_input_params_t *input_params, AmgErrorLogParams *black_params, amg_video_black_output_params_t *output_params)
{
    amg_video_black_context_t *amg_video_black = (amg_video_black_context_t*) handle;

    int wo = input_params->width;
    int ho = input_params->height;

    amg_video_roi_t  roi_black;

    if (!verify_roi(amg_video_black, wo, ho, &roi_black))
    {
        log_warn(amg_video_black->logger, "Invalid roi for given wo %d ho %d resetting roi", wo, ho);
    }

    amg_video_black_algo_params_t algo_params;
    algo_params.in_image=input_params->in_image;
    algo_params.roi_black=roi_black;
    algo_params.wo = wo;
    AmgErrorStatus input = amg_video_black->detect(amg_video_black->algo_handle,&algo_params);

    amg_video_black_fsm_input_params_t input_fsm_params;
    amg_video_black_fsm_output_params_t output_fsm_params;
    input_fsm_params.buffer_pts=input_params->buffer_pts;
    input_fsm_params.input = input;
    output_fsm_params.start_pts = 0;
    output_fsm_params.end_pts = 0;
    amg_video_black_fsm_detect(amg_video_black->fsm_handle, &input_fsm_params, &output_fsm_params);
    if (black_params != NULL)
    {
        black_params->status = output_fsm_params.is_black;
        black_params->duration_s = output_fsm_params.duration_ms/1000;
    }
    if(output_params !=NULL)
    {
        output_params->start_pts = output_fsm_params.start_pts;
        output_params->end_pts = output_fsm_params.end_pts;
        output_params->duration_ms = output_fsm_params.duration_ms;
    }

    return 0;
}

void amg_video_black_reset(void *handle)
{
    amg_video_black_context_t *amg_video_black = ( amg_video_black_context_t *)handle;
    amg_video_black_fsm_reset( amg_video_black->fsm_handle );

}
