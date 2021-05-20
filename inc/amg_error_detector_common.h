#ifndef __AMG_ERROR_DETECTOR_COMMON_H__
#define __AMG_ERROR_DETECTOR_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

    typedef enum _AmgErrorStatus
    {
        ERROR_FALSE,
        ERROR_TRUE,
        ERROR_NA = -1

    } AmgErrorStatus;

    typedef struct  _AmgErrorLogParams
    {
        AmgErrorStatus  status;
        int64_t         duration_s;

    } AmgErrorLogParams;

#ifdef __cplusplus
}
#endif
#endif //__AMG_ERROR_DETECTOR_COMMON_H__
