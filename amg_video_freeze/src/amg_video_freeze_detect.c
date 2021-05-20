/** @file amg_video_freeze_detect_fsm.c
 *  @brief Amagi Freeze Detector
 *
 *  @author Viswanath Srikanth Bathina (viswanath.b@amagi.com)
 */

#include <unistd.h>
#include "amg_video_freeze_detect.h"
#include <amg_video_freeze_detect_fsm.h>
#include <amg_video_freeze_detect_algo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define MAX_LOGGER_NAME_LEN 64
typedef void (*fn_algo_deinit) (void* handle);
typedef int  (*fn_algo_detect) (void *handle, amg_video_freeze_algo_params_t *params);

typedef struct __amg_video_freeze_context_t
{
    int                     silent;
    void                    *logger;
    amg_video_roi_t         roi_freeze;
    void                    *fsm_handle;
    void                    *algo_handle;
    fn_algo_detect          detect;
    fn_algo_deinit          deinit;
    uint8_t                 *old_image;
    int                     old_image_size;

} amg_video_freeze_context_t;

void* amg_video_freeze_init(amg_video_freeze_init_params_t *init_params)
{
    amg_video_freeze_context_t * amg_video_freeze = (amg_video_freeze_context_t *) malloc(sizeof(amg_video_freeze_context_t));
    char logger_name[MAX_LOGGER_NAME_LEN];

    snprintf (logger_name, MAX_LOGGER_NAME_LEN, "com.amagi.amg_video_freezeDetector");
    amg_video_freeze->logger = log_init (logger_name);
    amg_video_freeze->silent = init_params->silent;
    amg_video_freeze->old_image = NULL;
    amg_video_freeze->old_image_size  = 0;
    memcpy(&amg_video_freeze->roi_freeze, &init_params->freeze_detect_roi, sizeof(amg_video_roi_t));
    
    amg_video_freeze_algo_init_params_t algo_init_params;
    algo_init_params.silent = init_params->silent;
    algo_init_params.skip_line = init_params->skip_line;
    algo_init_params.freeze_detect_threshold_y = init_params->freeze_detect_threshold;
    algo_init_params.freeze_detect_threshold_uv = init_params->freeze_detect_threshold_uv;
    amg_video_freeze->algo_handle = amg_video_freeze_algo_init(&algo_init_params);
    amg_video_freeze->detect = amg_video_freeze_algo_detect;
    amg_video_freeze->deinit = amg_video_freeze_algo_deinit;
    amg_video_freeze_fsm_init_params_t fsm_init_params;
    fsm_init_params.freeze_to_no_freeze_threshold_ms = init_params->freeze_to_no_freeze_threshold_ms;
    fsm_init_params.no_freeze_to_freeze_threshold_ms = init_params->no_freeze_to_freeze_threshold_ms;
    fsm_init_params.intermediate_freeze_to_no_freeze_frames_threshold = init_params->intermediate_freeze_to_no_freeze_frames_threshold;
    fsm_init_params.pts_interval_ns = init_params->pts_interval_ns;
    fsm_init_params.silent = init_params->silent;
    amg_video_freeze->fsm_handle = amg_video_freeze_fsm_init(&fsm_init_params);
    return (void *) amg_video_freeze;
}

void amg_video_freeze_deinit(void *handle)
{
    amg_video_freeze_context_t *amg_video_freeze = (amg_video_freeze_context_t *)handle;

    if(amg_video_freeze)
    {
        amg_video_freeze_fsm_deinit(amg_video_freeze->fsm_handle);
        amg_video_freeze->deinit(amg_video_freeze->algo_handle);
        log_deinit(amg_video_freeze->logger);
        free(amg_video_freeze);
    }
}

static bool verify_roi(amg_video_freeze_context_t *amg_video_freeze, int wo, int ho,
        amg_video_roi_t *roi_freeze)
{
    if (amg_video_freeze->roi_freeze.w + amg_video_freeze->roi_freeze.x > wo ||
            amg_video_freeze->roi_freeze.h + amg_video_freeze->roi_freeze.y > ho)
    {
        roi_freeze->x = roi_freeze->y = 0;
        roi_freeze->w = wo;
        roi_freeze->h = ho;
        return false;
    }
    if (amg_video_freeze->roi_freeze.w == 0 || amg_video_freeze->roi_freeze.h == 0)
    {
        roi_freeze->x = 0;
        roi_freeze->y = 0;
        roi_freeze->w = wo;
        roi_freeze->h = ho;
    }
    else
    {
        memcpy(roi_freeze, &amg_video_freeze->roi_freeze, sizeof(amg_video_roi_t));
    }
    return true;
}

int amg_video_freeze_detect(void * handle, amg_video_freeze_input_params_t *input_params, AmgErrorLogParams *freeze_params, amg_video_freeze_output_params_t *output_params)
{
    amg_video_freeze_context_t *amg_video_freeze = (amg_video_freeze_context_t*) handle;

    int wo = input_params->width;
    int ho = input_params->height;

    amg_video_roi_t  roi_freeze;

    if (!verify_roi(amg_video_freeze, wo, ho, &roi_freeze))
    {
        log_warn(amg_video_freeze->logger, "Invalid roi for given wo %d ho %d resetting roi", wo, ho);
    }
    if (amg_video_freeze->old_image && amg_video_freeze->old_image_size == wo * ho)
    {
        amg_video_freeze_algo_params_t algo_params;
        algo_params.in_image=input_params->in_image;
        algo_params.old_image=amg_video_freeze->old_image;
        algo_params.roi_freeze=roi_freeze;
        algo_params.color_type = input_params->color_type;
        algo_params.wo = wo;
        algo_params.ho = ho;
        AmgErrorStatus input = amg_video_freeze->detect(amg_video_freeze->algo_handle, &algo_params);

        amg_video_freeze_fsm_input_params_t input_fsm_params;
        amg_video_freeze_fsm_output_params_t output_fsm_params;
        input_fsm_params.buffer_pts=input_params->buffer_pts;
        input_fsm_params.input = input;
        output_fsm_params.start_pts = 0;
        output_fsm_params.end_pts = 0;
        amg_video_freeze_fsm_detect(amg_video_freeze->fsm_handle, &input_fsm_params, &output_fsm_params);
        if (freeze_params != NULL)
        {
            freeze_params->status = output_fsm_params.is_freeze;
            freeze_params->duration_s = output_fsm_params.duration_ms/1000;
        }
        if (output_params !=NULL)
        {
            output_params->start_pts = output_fsm_params.start_pts;
            output_params->end_pts = output_fsm_params.end_pts;
            output_params->duration_ms = output_fsm_params.duration_ms;
        }
        if (output_fsm_params.is_update_old_image && output_fsm_params.is_freeze == ERROR_FALSE)
        {
            if(input_params->color_type == FORMAT_I420)
                memcpy(amg_video_freeze->old_image, input_params->in_image, (size_t)((wo * ho * 3) / 2));
            else
                memcpy(amg_video_freeze->old_image, input_params->in_image, (size_t)(wo * ho));
        }
    }
    else
    {
        if(amg_video_freeze->old_image)
        {
            free(amg_video_freeze->old_image);
        }
        amg_video_freeze->old_image = malloc((size_t)(wo*ho*3/2));
        if(input_params->color_type == FORMAT_I420)
            memcpy(amg_video_freeze->old_image, input_params->in_image, (size_t)((wo * ho * 3) / 2));
        else
            memcpy(amg_video_freeze->old_image, input_params->in_image, (size_t)(wo * ho));
        amg_video_freeze->old_image_size = wo*ho;
    }
    return 0;
}

void amg_video_freeze_reset(void *handle)
{
    amg_video_freeze_context_t *amg_video_freeze = ( amg_video_freeze_context_t *)handle;
    amg_video_freeze_fsm_reset( amg_video_freeze->fsm_handle );
    if(amg_video_freeze->old_image)
    {
        free(amg_video_freeze->old_image);
        amg_video_freeze->old_image   = NULL;
        amg_video_freeze->old_image_size  = 0;
    }
}

