/** @file amg_video_black_detect_slice_algo.h
 *  @brief Amagi Black Detector slice_slice_algo
 *
 *  @author Viswanath Srikanth Bathina (viswanath.b@amagi.com)
 */
#ifndef AMG_VIDEO_BLACK_SLICE_ALGO
#define AMG_VIDEO_BLACK_SLICE_ALGO

#include "ipp.h"
#include "amg_video_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief black frames detection
 *
 * @param image
 * @param region of interest
 * @param number of lines to skip
 * @param line offset
 *
 *  @return int 1 if there is black else 0.
 */

typedef struct __amg_video_black_slice_algo_init_params_t
{
    int     silent;
    int     skip_line;
    double  black_frames_threshold_y;
    int     no_of_slices;
} amg_video_black_slice_algo_init_params_t;


/** @brief Initialize amg_video_black_fsm.
 *
 * This will initialize the amg_video_black_fsm context.
 *
 *
 * @return void* pointer which holds handle for amg_video_black.
 */
void *amg_video_black_slice_algo_init(amg_video_black_slice_algo_init_params_t *init_params);

/** @brief black frames detection
 *
 *  @param handle context
 *  @params image and roi
 *
 *  @return ERROR_TRUE for black frames and ERROR_FALSE for non black frames
 */
AmgErrorStatus amg_video_black_slice_algo_detect(void *handle, amg_video_black_algo_params_t *params);

/** @brief Deinitialize
 *
 * @param handle pointer which holds handle for amg_video_black_fsm.
 */
void amg_video_black_slice_algo_deinit(void *handle);

#ifdef __cplusplus
}
#endif 

#endif //AMG_VIDEO_BLACK_SLICE_ALGO

