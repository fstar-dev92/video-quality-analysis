/** @file amg_video_black_detect.h
 *  @brief Amagi Black Detector
 *
 *  @author Viswanath Srikanth Bathina (viswanath.b@amagi.com)
 */
#ifndef AMG_VIDEO_BLACK_DETECT
#define AMG_VIDEO_BLACK_DETECT

#include "amg_error_detector_common.h"
#include "ipp.h"
#include "amg_video_common.h"
#ifdef __cplusplus
extern "C" {
#endif

/** @brief structure used for amg_video_blackProcess call.
 *
 */
typedef enum _amg_video_black_algo_type
{
    AMG_VIDEO_BLACK_ALGO_TYPE,
    AMG_VIDEO_BLACK_SLICE_ALGO_TYPE,
}amg_video_black_algo_type;

typedef struct __amg_video_black_init_params_t
{
    int                     silent; // For Logging
    int                     no_black_to_black_threshold_ms;
    int                     black_to_no_black_threshold_ms;
    double                  black_frames_threshold; // Default value 5
    amg_video_roi_t         black_detect_roi;
    int                     skip_line;
    int64_t                 pts_interval_ns;
    int                     no_of_slices;
    amg_video_black_algo_type   algo_type;
} amg_video_black_init_params_t;

typedef struct __amg_video_black_input_params_t
{
    uint8_t     *in_image;
    int         width;
    int         height;
    uint64_t    buffer_pts; //Used for periodic Logging
    int         color_type;
} amg_video_black_input_params_t;

typedef struct __amg_video_black_output_params_t
{
    int64_t start_pts;
    int64_t end_pts;
    int64_t duration_ms;
} amg_video_black_output_params_t;


/** @brief Initialize amg_video_black.
 *
 * This will initialize the amg_video_black context.
 *
 * @param init_params: Initialize parameters
 *
 * @return void* pointer which holds handle for amg_video_black.
 */
void* amg_video_black_init(amg_video_black_init_params_t *init_params);

/** @brief black frames detection
 *
 *  @param handle context
 *  @param params input params
 *  @param is_black_frame Set by library
 *  @return int -1 if there is error in conversion else 0.
 */
int amg_video_black_detect(void * handle, amg_video_black_input_params_t *input_params, AmgErrorLogParams *black_params, amg_video_black_output_params_t *output_params);

/** @brief Deinitialize
 *
 * @param handle pointer which holds handle for amg_video_black.
 */
void amg_video_black_deinit(void * handle);

/** @brief Reset state information 
 *
 * @param handle pointer which holds handle for amg_video_black.
 */
void amg_video_black_reset(void *handle);
#ifdef __cplusplus
}
#endif 

#endif //AMG_VIDEO_BLACK_DETECT

