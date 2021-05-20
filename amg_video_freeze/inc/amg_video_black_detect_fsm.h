/** @file amg_video_black_detect.h
 *  @brief Amagi Black Detector
 *
 *  @author Viswanath Srikanth Bathina (viswanath.b@amagi.com)
 */
#ifndef AMG_VIDEO_BLACK_FSM
#define AMG_VIDEO_BLACK_FSM

#include "amg_error_detector_common.h"
#include "ipp.h"
#include "amg_video_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __amg_video_black_fsm_init_params_t
{
    int     silent;
    double  no_black_to_black_threshold_ms;
    double  black_to_no_black_threshold_ms;
    int64_t  pts_interval_ns;
} amg_video_black_fsm_init_params_t;

typedef struct __amg_video_black_fsm_input_params_t
{
    uint64_t        buffer_pts; //curent frame pts
    AmgErrorStatus  input;
} amg_video_black_fsm_input_params_t;

typedef struct __amg_video_black_fsm_output_params_t
{
    int is_black;
    int64_t start_pts;  //keeping track of black pts 
    int64_t end_pts;    //keeping track of black pts
    int64_t duration_ms;//keeping track of current state pts
} amg_video_black_fsm_output_params_t;

/** @brief Initialize amg_video_black_fsm.
 *
 * This will initialize the amg_video_black_fsm context.
 *
 *
 * @return void* pointer which holds handle for amg_video_black.
 */
void* amg_video_black_fsm_init(amg_video_black_fsm_init_params_t *init_params);

/** @brief black frames detection
 *
 *  @param handle context
 *  @param pts and input (black or non-black) 
 */
void amg_video_black_fsm_detect(void *handle, amg_video_black_fsm_input_params_t *input_params, amg_video_black_fsm_output_params_t *output_params);    

/** @brief Deinitialize
 *
 * @param handle pointer which holds handle for amg_video_black_fsm.
 */
void amg_video_black_fsm_deinit(void *handle);

/** @brief Reset state information 
 *
 * @param handle pointer which holds handle for amg_video_black_fsm.
 */
void amg_video_black_fsm_reset(void *handle);
#ifdef __cplusplus
}
#endif

#endif //AMG_VIDEO_BLACK_FSM

