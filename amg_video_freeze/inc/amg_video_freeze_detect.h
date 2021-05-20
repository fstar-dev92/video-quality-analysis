/** @file amg_video_freeze_detect.h
 *  @brief Amagi Freeze Detector
 *
 *  @author Viswanath Srikanth Bathina (viswanath.b@amagi.com)
 */
#ifndef AMG_VIDEO_FREEZE_DETECT
#define AMG_VIDEO_FREEZE_DETECT

#include "amg_error_detector_common.h"
#include "amg_video_common.h"
#include "ipp.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief structure used for amg_video_freezeProcess call.
 *
 */
typedef struct __amg_video_freeze_init_params_t{
    int             silent; // For Logging
    int             no_freeze_to_freeze_threshold_ms;
    int             freeze_to_no_freeze_threshold_ms;
    double          freeze_detect_threshold; // Default value 2
    double          freeze_detect_threshold_uv; // Default value 1.2
    amg_video_roi_t freeze_detect_roi;
    int             skip_line;
    int64_t         pts_interval_ns;
    int             intermediate_freeze_to_no_freeze_frames_threshold;
} amg_video_freeze_init_params_t;

typedef struct __amg_video_freeze_input_params_t
{
    uint8_t     *in_image;
    int         width;
    int         height;
    uint64_t    buffer_pts; //Used for periodic Logging
    int         color_type;
} amg_video_freeze_input_params_t;

typedef struct __amg_video_freeze_output_params_t
{
    int64_t start_pts;
    int64_t end_pts;
    int64_t duration_ms;
} amg_video_freeze_output_params_t;

/** @brief Initialize amg_video_freeze.
 *
 * This will initialize the amg_video_freeze context.
 *
 * @param init_params: Initialize parameters
 *
 * @return void* pointer which holds handle for amg_video_freeze.
 */
void * amg_video_freeze_init(amg_video_freeze_init_params_t *init_params);

/** @brief Freeze frames detection
 *
 *  @param handle context
 *  @param params input params
 *  @param is_black_frame, is_freeze Sets by library
 *  @return int -1 if there is error in conversion else 0.
 */
int amg_video_freeze_detect(void * handle, amg_video_freeze_input_params_t *input_params, AmgErrorLogParams *freeze_params, amg_video_freeze_output_params_t *output_params);

/** @brief Deinitialize
 *
 * @param handle pointer which holds handle for amg_video_freeze.
 */
void amg_video_freeze_deinit(void * handle);

/** @brief Reset amg_video_freeze
 *
 * @param handle pointer which holds handle for amg_video_freeze.
 */
void amg_video_freeze_reset(void * handle);

#ifdef __cplusplus
}
#endif

#endif //AMG_VIDEO_FREEZE_DETECT
