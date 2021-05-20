/** @file amg_video_freeze_detect_algo.h
 *  @brief Amagi Freeze Detector Algo
 *
 *  @author Viswanath Srikanth Bathina (viswanath.b@amagi.com)
 */
#ifndef AMG_VIDEO_FREEZE_ALGO
#define AMG_VIDEO_FREEZE_ALGO

#include "ipp.h"
#include "amg_video_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief freeze frames detection
 *
 * @param image
 * @param region of interest
 * @param number of lines to skip
 * @param line offset
 *
 *  @return int 1 if there is freeze else 0.
 */

typedef struct __amg_video_freeze_algo_init_params_t
{
    int     silent;
    int     skip_line;
    double  freeze_detect_threshold_y;
    double  freeze_detect_threshold_uv;
} amg_video_freeze_algo_init_params_t;

/** @brief Initialize amg_video_freeze_fsm.
 *
 * This will initialize the amg_video_freeze_fsm context.
 *
 *
 * @return void* pointer which holds handle for amg_video_freeze.
 */
void *amg_video_freeze_algo_init(amg_video_freeze_algo_init_params_t *init_params);

/** @brief freeze frames detection
 *
 *  @param handle context
 *  @params image and roi
 *
 *  @return ERROR_TRUE for freeze frames and ERROR_FALSE for non freeze frames
 */
AmgErrorStatus amg_video_freeze_algo_detect(void *handle, amg_video_freeze_algo_params_t *params);

/** @brief Deinitialize
 *
 * @param handle pointer which holds handle for amg_video_freeze_fsm.
 */
void amg_video_freeze_algo_deinit(void *handle);

#ifdef __cplusplus
}
#endif 

#endif //AMG_VIDEO_FREEZE_ALGO

