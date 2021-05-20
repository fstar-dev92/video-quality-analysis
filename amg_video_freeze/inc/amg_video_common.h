#ifndef AMG_ERROR_VIDEO_COMMON
#define AMG_ERROR_VIDEO_COMMON
#ifdef __cplusplus
extern "C" {
#endif
#ifdef AMG_MIMAS
#include "amg_logger.h"
#include "mimas_common.h"
#define log_init(...)   log4cxx_init (__VA_ARGS__)
#define log_info(...)   log4cxx_info ( __VA_ARGS__)
#define log_warn(...)   log4cxx_warn ( __VA_ARGS__)
#define log_error(...)  log4cxx_error ( __VA_ARGS__)
#define log_fatal(...)  log4cxx_fatal ( __VA_ARGS__)
#define log_debug(...)  log4cxx_debug (__VA_ARGS__)
#define log_trace(...)  log4cxx_trace ( __VA_ARGS__)
#define log_deinit(...) log4cxx_deinit ( __VA_ARGS__)
#else
#include "stdbool.h"
#include "stdint.h"
#define _EXCLUDE_FIRST_ARG_(first, ...) __VA_ARGS__
#define EXCLUDE_FIRST_ARG(...) _EXCLUDE_FIRST_ARG_(__VA_ARGS__)
#define log_init(...)   NULL
#define log_info(...)   printf ("INFO: " EXCLUDE_FIRST_ARG(__VA_ARGS__)), printf ("\n")
#define log_warn(...)   printf ("WARN: " EXCLUDE_FIRST_ARG(__VA_ARGS__)), printf ("\n")
#define log_error(...)  printf ("ERROR: " EXCLUDE_FIRST_ARG(__VA_ARGS__)), printf ("\n")
#define log_fatal(...)  printf ("FATAL: " EXCLUDE_FIRST_ARG(__VA_ARGS__)), printf ("\n")
#define log_debug(...)  printf ("DEBUG: " EXCLUDE_FIRST_ARG(__VA_ARGS__)), printf ("\n")
#define log_trace(...)  printf ("TRACE: " EXCLUDE_FIRST_ARG(__VA_ARGS__)), printf ("\n")
#define log_deinit(...) NULL
#define MS_TO_NS_CONVERSION ( 1000000 ) // Conversion from ms to ns
#define S_TO_NS_CONVERSION ( 1000000000L ) // Conversion from s to ns
    //Video Format Supported
    enum {
        FORMAT_UNDEFINED    = 0,
        FORMAT_Y42B         = 1,
        FORMAT_NV12         = 2,
        FORMAT_UYVY422      = 3,
        FORMAT_YUYV422      = 4,
        FORMAT_I420         = 5,
        FORMAT_ARGB         = 6,
        FORMAT_I420_10LE    = 7,
        FORMAT_Y42B_10LE    = 8
    };

#endif

typedef struct __amg_video_roi_t{
    int x;
    int y;
    int w;
    int h;
} amg_video_roi_t;

typedef struct __amg_video_black_algo_params_t
{
    uint8_t         *in_image;
    amg_video_roi_t roi_black;
    int             wo;//stride
} amg_video_black_algo_params_t;

typedef struct __amg_video_freeze_algo_params_t
{
    uint8_t         *in_image;
    uint8_t         *old_image;
    amg_video_roi_t roi_freeze;
    int             color_type;
    int             wo;//stride
    int             ho;//for calculation of offset for uv
} amg_video_freeze_algo_params_t;

#ifdef __cplusplus
}
#endif

#endif //AMG_ERROR_VIDEO_COMMON
