/** @file amg_video_freeze_detect.h
 *  @brief Amagi Freeze Detector
 *
 *  @author Viswanath Srikanth Bathina (viswanath.b@amagi.com)
 */
#ifndef AMG_VIDEO_FREEZE_FSM
#define AMG_VIDEO_FREEZE_FSM

#include "amg_error_detector_common.h"
#include "amg_video_common.h"
#include "ipp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __amg_video_freeze_fsm_init_params_t
{
    int     silent;
    double  no_freeze_to_freeze_threshold_ms;
    double  freeze_to_no_freeze_threshold_ms;
    int     intermediate_freeze_to_no_freeze_frames_threshold;
    int64_t  pts_interval_ns;
} amg_video_freeze_fsm_init_params_t;

typedef struct __amg_video_freeze_fsm_input_params_t
{
    uint64_t        buffer_pts; //curent frame pts
    AmgErrorStatus  input;
} amg_video_freeze_fsm_input_params_t;

typedef struct __amg_video_freeze_fsm_output_params_t
{
    int     is_freeze;
    bool    is_update_old_image;
    int64_t start_pts;  //keeping track of freeze pts 
    int64_t end_pts;    //keeping track of freeze pts
    int64_t duration_ms;//keeping track of current state pts
} amg_video_freeze_fsm_output_params_t;

/** @brief Initialize amg_video_freeze_fsm.
 *
 * This will initialize the amg_video_freeze_fsm context.
 *
 *
 * @return void* pointer which holds handle for amg_video_freeze.
 */
void* amg_video_freeze_fsm_init(amg_video_freeze_fsm_init_params_t *init_params);

/** @brief freeze frames detection
 *
 *  @param handle context
 *  @param pts and input (freeze or non-freeze) 
 */
void amg_video_freeze_fsm_detect(void *handle, amg_video_freeze_fsm_input_params_t *input_params, amg_video_freeze_fsm_output_params_t *output_params);    

/** @brief Deinitialize
 *
 * @param handle pointer which holds handle for amg_video_freeze_fsm.
 */
void amg_video_freeze_fsm_deinit(void *handle);

/** @brief Reset state information 
 *
 * @param handle pointer which holds handle for amg_video_freeze_fsm.
 */
void amg_video_freeze_fsm_reset(void *handle);
#ifdef __cplusplus
}
#endif

#endif //AMG_VIDEO_FREEZE_FSM


