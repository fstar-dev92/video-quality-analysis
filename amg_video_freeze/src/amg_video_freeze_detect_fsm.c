/** @file amg_video_freeze_detect_fsm.c
 *  @brief Amagi Freeze Detector
 *
 *  @author Viswanath Srikanth Bathina (viswanath.b@amagi.com)
 */

#include <unistd.h>
#include <amg_video_freeze_detect_fsm.h>
#include <amg_video_state_detect.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LOGGER_NAME_LEN 64

typedef enum _amg_video_freeze_state
{
    AMG_VIDEO_NON_FREEZE_STATE,
    AMG_VIDEO_FREEZE_STATE,
}amg_video_freeze_state;

typedef struct __amg_video_freeze_fsm_context_t
{
    int     silent;
    void    *logger;
    void    *fsm_handle;
    int64_t pts_interval_ns;
    int     adjacent_cur_state_frame_count;
    int     intermediate_frames_threshold[2];
} amg_video_freeze_fsm_context_t;

void* amg_video_freeze_fsm_init(amg_video_freeze_fsm_init_params_t *params)
{
    amg_video_freeze_fsm_context_t *amg_video_freeze_fsm = ( amg_video_freeze_fsm_context_t *) malloc(sizeof(amg_video_freeze_fsm_context_t));
    amg_video_freeze_fsm->pts_interval_ns = params->pts_interval_ns;
    amg_video_freeze_fsm->intermediate_frames_threshold[AMG_VIDEO_NON_FREEZE_STATE] = params->intermediate_freeze_to_no_freeze_frames_threshold;
    amg_video_freeze_fsm->intermediate_frames_threshold[AMG_VIDEO_FREEZE_STATE] = 0;
    amg_video_freeze_fsm->adjacent_cur_state_frame_count = 0;

    char logger_name[MAX_LOGGER_NAME_LEN];
    snprintf (logger_name, MAX_LOGGER_NAME_LEN, "com.amagi.amg_video_freeze_fsmDetector");
    amg_video_freeze_fsm->logger = log_init (logger_name);
    amg_video_freeze_fsm->silent = params->silent;

    int state_cnt = 2;
    int input_cnt = 2;
    amg_video_freeze_fsm->fsm_handle = AmgVideoStates_Init(state_cnt,input_cnt);
    int threshold[2][2] = {{0,(int)(((int64_t)params->no_freeze_to_freeze_threshold_ms * MS_TO_NS_CONVERSION)/(params->pts_interval_ns))},
                            {(int)(((int64_t)params->freeze_to_no_freeze_threshold_ms * MS_TO_NS_CONVERSION)/(params->pts_interval_ns)),0}};
    int state_transfer[2][2] = {{0,1},{0,1}};
    int i;
    for(i=0;i<state_cnt;i++)
    {
        AmgVideoStates_Add(amg_video_freeze_fsm->fsm_handle,i,threshold[i],state_transfer[i]);
    }
    return (void*)amg_video_freeze_fsm;
}

void  amg_video_freeze_fsm_detect(void *handle, amg_video_freeze_fsm_input_params_t *input_params, amg_video_freeze_fsm_output_params_t *output_params)  
{
    amg_video_freeze_fsm_context_t *amg_video_freeze_fsm = ( amg_video_freeze_fsm_context_t *)handle;
    int  cur_state_cnt, prev_state_cnt ; 
    amg_video_freeze_state prev_state, cur_state;

    prev_state = AmgVideoStates_GetState( amg_video_freeze_fsm->fsm_handle );
    if (input_params->input != (int)prev_state)
        amg_video_freeze_fsm->adjacent_cur_state_frame_count = 0;
    else
        amg_video_freeze_fsm->adjacent_cur_state_frame_count++;

    if (amg_video_freeze_fsm->adjacent_cur_state_frame_count <= amg_video_freeze_fsm->intermediate_frames_threshold[prev_state] )
    {
        input_params->input = 1 ^ prev_state;
        output_params->is_update_old_image = 0;
    }
    else 
        output_params->is_update_old_image = 1;

    AmgVideoStates_Input( amg_video_freeze_fsm->fsm_handle, input_params->input );
    cur_state = AmgVideoStates_GetState( amg_video_freeze_fsm->fsm_handle );
    cur_state_cnt = AmgVideoStates_GetCurStateCount(amg_video_freeze_fsm->fsm_handle);
    prev_state_cnt = AmgVideoStates_GetPrevStateCount(amg_video_freeze_fsm->fsm_handle);

    output_params->start_pts = (int64_t)( input_params->buffer_pts - (uint64_t)cur_state_cnt * (uint64_t)(amg_video_freeze_fsm->pts_interval_ns ) );
    if(prev_state == AMG_VIDEO_FREEZE_STATE && cur_state == AMG_VIDEO_NON_FREEZE_STATE)
    {
        output_params->end_pts = (int64_t)( input_params->buffer_pts - (uint64_t)cur_state_cnt * (uint64_t)(amg_video_freeze_fsm->pts_interval_ns) );
        output_params->start_pts = output_params->end_pts - (int64_t)prev_state_cnt * (int64_t)(amg_video_freeze_fsm->pts_interval_ns);
        output_params->is_update_old_image = 1;
    }
    output_params->is_freeze = ( cur_state == AMG_VIDEO_FREEZE_STATE ) ? ERROR_TRUE : ERROR_FALSE;
    output_params->duration_ms = cur_state_cnt * (int64_t)(amg_video_freeze_fsm->pts_interval_ns/MS_TO_NS_CONVERSION);
    
    if ( !amg_video_freeze_fsm->silent )
    {
        if(cur_state == AMG_VIDEO_FREEZE_STATE)
            log_info (amg_video_freeze_fsm->logger, "freeze_ns %ld no_freeze_ns %ld is_freeze %d", cur_state_cnt * (int64_t)(amg_video_freeze_fsm->pts_interval_ns), prev_state_cnt*(amg_video_freeze_fsm->pts_interval_ns), output_params->is_freeze);
        else
            log_info (amg_video_freeze_fsm->logger, "freeze_ns %ld no_freeze_ns %ld is_freeze %d", prev_state_cnt * (int64_t)(amg_video_freeze_fsm->pts_interval_ns), cur_state_cnt * (int64_t)(amg_video_freeze_fsm->pts_interval_ns), output_params->is_freeze);
    }
}
void amg_video_freeze_fsm_deinit(void *handle)
{
    amg_video_freeze_fsm_context_t *amg_video_freeze_fsm = ( amg_video_freeze_fsm_context_t *)handle;
    if( amg_video_freeze_fsm )
    {
        AmgVideoStates_DeInit(amg_video_freeze_fsm->fsm_handle);
        log_deinit(amg_video_freeze_fsm->logger);
        free( amg_video_freeze_fsm );
    }
}
void amg_video_freeze_fsm_reset(void *handle)
{
    amg_video_freeze_fsm_context_t *amg_video_freeze_fsm = ( amg_video_freeze_fsm_context_t *)handle;
    AmgVideoStates_Reset( amg_video_freeze_fsm->fsm_handle );

}

