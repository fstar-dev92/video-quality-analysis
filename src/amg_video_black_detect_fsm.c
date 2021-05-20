/** @file amg_video_black_detect_fsm.c
 *  @brief Amagi Black Detector
 *
 *  @author Viswanath Srikanth Bathina (viswanath.b@amagi.com)
 */

#include <unistd.h>
#include <amg_video_black_detect_fsm.h>
#include <amg_video_state_detect.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LOGGER_NAME_LEN 64

typedef enum _amg_video_black_state
{
    AMG_VIDEO_NON_BLACK_STATE,
    AMG_VIDEO_BLACK_STATE,
}amg_video_black_state;

typedef struct __amg_video_black_fsm_context_t
{
    int     silent;
    void    *logger;
    void    *fsm_handle;
    int64_t  pts_interval_ns;
} amg_video_black_fsm_context_t;

void* amg_video_black_fsm_init(amg_video_black_fsm_init_params_t *params)
{
    amg_video_black_fsm_context_t *amg_video_black_fsm = ( amg_video_black_fsm_context_t *) malloc(sizeof(amg_video_black_fsm_context_t));
    amg_video_black_fsm->pts_interval_ns = params->pts_interval_ns;

    char logger_name[MAX_LOGGER_NAME_LEN];
    snprintf (logger_name, MAX_LOGGER_NAME_LEN, "com.amagi.amg_video_black_fsmDetector");
    amg_video_black_fsm->logger = log_init (logger_name);
    amg_video_black_fsm->silent = params->silent;

    int state_cnt = 2;
    int input_cnt = 2;
    amg_video_black_fsm->fsm_handle = AmgVideoStates_Init(state_cnt,input_cnt);
    int threshold[2][2] = {{0,(int)(((int64_t)params->no_black_to_black_threshold_ms * MS_TO_NS_CONVERSION)/(params->pts_interval_ns))},
                            {(int)(((int64_t)params->black_to_no_black_threshold_ms * MS_TO_NS_CONVERSION)/(params->pts_interval_ns)),0}};
    int state_transfer[2][2] = {{0,1},{0,1}};
    int i;
    for(i=0;i<state_cnt;i++)
    {
        AmgVideoStates_Add(amg_video_black_fsm->fsm_handle,i,threshold[i],state_transfer[i]);
    }
    return (void*)amg_video_black_fsm;
}

void  amg_video_black_fsm_detect(void *handle, amg_video_black_fsm_input_params_t *input_params, amg_video_black_fsm_output_params_t *output_params)  
{
    amg_video_black_fsm_context_t *amg_video_black_fsm = ( amg_video_black_fsm_context_t *)handle;
    int  cur_state_cnt, prev_state_cnt ; 
    amg_video_black_state prev_state, cur_state;

    prev_state = AmgVideoStates_GetState( amg_video_black_fsm->fsm_handle );
    AmgVideoStates_Input( amg_video_black_fsm->fsm_handle, input_params->input );
    cur_state = AmgVideoStates_GetState( amg_video_black_fsm->fsm_handle );
    cur_state_cnt = AmgVideoStates_GetCurStateCount(amg_video_black_fsm->fsm_handle);
    prev_state_cnt = AmgVideoStates_GetPrevStateCount(amg_video_black_fsm->fsm_handle);

    output_params->start_pts = (int64_t)( input_params->buffer_pts - (uint64_t)cur_state_cnt * (uint64_t)(amg_video_black_fsm->pts_interval_ns ) );
    if(prev_state == AMG_VIDEO_BLACK_STATE && cur_state == AMG_VIDEO_NON_BLACK_STATE)
    {
        output_params->end_pts = (int64_t)( input_params->buffer_pts - (uint64_t)cur_state_cnt * (uint64_t)(amg_video_black_fsm->pts_interval_ns) );
        output_params->start_pts = output_params->end_pts - (int64_t)prev_state_cnt * (int64_t)(amg_video_black_fsm->pts_interval_ns);
    }
    output_params->is_black = ( cur_state == AMG_VIDEO_BLACK_STATE ) ? ERROR_TRUE : ERROR_FALSE;
    output_params->duration_ms = cur_state_cnt * (int64_t)(amg_video_black_fsm->pts_interval_ns/MS_TO_NS_CONVERSION);
    
    if ( !amg_video_black_fsm->silent )
    {
        if(cur_state == AMG_VIDEO_BLACK_STATE)
            log_info(amg_video_black_fsm->logger, "black_ns %ld no_black_ns %ld is_black %d", cur_state_cnt * (int64_t)(amg_video_black_fsm->pts_interval_ns), prev_state_cnt*(amg_video_black_fsm->pts_interval_ns), output_params->is_black);
        else
            log_info(amg_video_black_fsm->logger, "black_ns %ld no_black_ns %ld is_black %d", prev_state_cnt * (int64_t)(amg_video_black_fsm->pts_interval_ns), cur_state_cnt * (int64_t)(amg_video_black_fsm->pts_interval_ns), output_params->is_black);
    }
}
void amg_video_black_fsm_deinit(void *handle)
{
    amg_video_black_fsm_context_t *amg_video_black_fsm = ( amg_video_black_fsm_context_t *)handle;
    if( amg_video_black_fsm )
    {
        AmgVideoStates_DeInit(amg_video_black_fsm->fsm_handle);
        log_deinit(amg_video_black_fsm->logger);
        free( amg_video_black_fsm );
    }
}
void amg_video_black_fsm_reset(void *handle)
{
    amg_video_black_fsm_context_t *amg_video_black_fsm = ( amg_video_black_fsm_context_t *)handle;
    AmgVideoStates_Reset( amg_video_black_fsm->fsm_handle );

}
