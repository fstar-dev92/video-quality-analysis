/** @file amg_video_freeze_detect_fsm.c
 *  @brief Amagi Freeze Detector
 *
 *  @author Viswanath Srikanth Bathina (viswanath.b@amagi.com)
 */

#include <unistd.h>
#include <amg_video_freeze_detect_algo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LOGGER_NAME_LEN 64

typedef struct __amg_video_freeze_algo_context_t
{
    int             silent;
    int             line_distance_y;
    int             line_distance_uv;
    int             line_offset;
    void            *logger;
    double          freeze_detect_threshold_y;
    double          freeze_detect_threshold_uv;
} amg_video_freeze_algo_context_t;

void* amg_video_freeze_algo_init(amg_video_freeze_algo_init_params_t *init_params)
{

    amg_video_freeze_algo_context_t * amg_video_freeze_algo = (amg_video_freeze_algo_context_t *) malloc(sizeof(amg_video_freeze_algo_context_t));
    char logger_name[MAX_LOGGER_NAME_LEN];

    snprintf (logger_name, MAX_LOGGER_NAME_LEN, "com.amagi.amg_video_freeze_algo");
    amg_video_freeze_algo->logger = log_init (logger_name);
    amg_video_freeze_algo->silent = init_params->silent;

    if (init_params->skip_line < 0)
    {
        log_warn(amg_video_freeze_algo->logger, "Minimum skip_line can be 0");
        amg_video_freeze_algo->line_distance_y = 1;
        amg_video_freeze_algo->line_distance_uv = 1;
    }
    else if (init_params->skip_line > 5)
    {
        log_warn(amg_video_freeze_algo->logger, "Maximum skip_line can be 5");
        amg_video_freeze_algo->line_distance_y = 6;
        amg_video_freeze_algo->line_distance_uv = 3;
    }
    else
    {
        amg_video_freeze_algo->line_distance_y = init_params->skip_line + 1;
        amg_video_freeze_algo->line_distance_uv = (init_params->skip_line / 2) + 1;
    }
    amg_video_freeze_algo->line_offset = 0;
    amg_video_freeze_algo->freeze_detect_threshold_y = init_params->freeze_detect_threshold_y;
    amg_video_freeze_algo->freeze_detect_threshold_uv = init_params->freeze_detect_threshold_uv;
    return (void*)amg_video_freeze_algo;
}
AmgErrorStatus amg_video_freeze_algo_detect(void *handle, amg_video_freeze_algo_params_t *params)
{
    amg_video_freeze_algo_context_t *amg_video_freeze_algo = (amg_video_freeze_algo_context_t *)handle;
    Ipp32f denom_freeze = sqrtf((Ipp32f)(params->roi_freeze.w * params->roi_freeze.h / amg_video_freeze_algo->line_distance_y));

    Ipp64f l2_diff_y;
    Ipp64f l2_diff_u;
    Ipp64f l2_diff_v;
    Ipp64f rmse_freeze_y;
    Ipp64f rmse_freeze_u;
    Ipp64f rmse_freeze_v;
    IppiSize roi_size_freeze = {params->roi_freeze.w, params->roi_freeze.h / amg_video_freeze_algo->line_distance_y};

    /*rmse for freeze detection is calculated as, rmse(t)=ld_2(t)/\sqrt{|I(t)|}
     * where I(t) is current image, ld_2(t) is L2 norm for difference of I(t) and I(t-1), |X| operation gives cardinal value of X*/

    int offset_freeze   = (params->roi_freeze.y + amg_video_freeze_algo->line_offset) * params->wo + params->roi_freeze.x;
    
    ippiNormDiff_L2_8u_C1R(params->in_image + offset_freeze, params->wo * amg_video_freeze_algo->line_distance_y, params->old_image + offset_freeze,
            params->wo * amg_video_freeze_algo->line_distance_y, roi_size_freeze, &l2_diff_y);
    rmse_freeze_y = l2_diff_y / (denom_freeze);
    if(params->color_type == FORMAT_I420)
    {

        Ipp32f denom_freeze_uv = sqrtf((Ipp32f)(((params->roi_freeze.w * params->roi_freeze.h )/ amg_video_freeze_algo->line_distance_uv)))/2;
        IppiSize roi_size_freeze_uv = {params->roi_freeze.w / 2, (params->roi_freeze.h / 2) / amg_video_freeze_algo->line_distance_uv};
        int offset_freeze_u   = params->wo * params->ho + ((params->roi_freeze.y+ amg_video_freeze_algo->line_offset) / 2) * (params->wo / 2) + params->roi_freeze.x / 2;
        int offset_freeze_v   = params->wo * params->ho + (params->wo * params->ho) / 4 + ((params->roi_freeze.y + amg_video_freeze_algo->line_offset) / 2) * (params->wo / 2) + params->roi_freeze.x / 2;
        ippiNormDiff_L2_8u_C1R(params->in_image + offset_freeze_u, (params->wo / 2) * amg_video_freeze_algo->line_distance_uv, params->old_image + offset_freeze_u,
                (params->wo / 2) * amg_video_freeze_algo->line_distance_uv, roi_size_freeze_uv, &l2_diff_u);
        rmse_freeze_u = l2_diff_u / (denom_freeze_uv);

        ippiNormDiff_L2_8u_C1R(params->in_image + offset_freeze_v, (params->wo / 2) * amg_video_freeze_algo->line_distance_uv, params->old_image + offset_freeze_v,
                (params->wo / 2) * amg_video_freeze_algo->line_distance_uv, roi_size_freeze_uv, &l2_diff_v);
        rmse_freeze_v = l2_diff_v / (denom_freeze_uv);
    }
    if (!amg_video_freeze_algo->silent)
    {
        log_info(amg_video_freeze_algo->logger, "freeze_rms_y: %lf, freeze_rms_u:%lf, freeze_rms_v: %lf ", rmse_freeze_y, rmse_freeze_u, rmse_freeze_v);
    }
    if(rmse_freeze_y <= amg_video_freeze_algo->freeze_detect_threshold_y &&
            rmse_freeze_u <= amg_video_freeze_algo->freeze_detect_threshold_uv &&
            rmse_freeze_v <= amg_video_freeze_algo->freeze_detect_threshold_uv)
        return ERROR_TRUE;
    return ERROR_FALSE;
}

void amg_video_freeze_algo_deinit(void *handle)
{
    amg_video_freeze_algo_context_t *amg_video_freeze_algo = (amg_video_freeze_algo_context_t *)handle;

    if (amg_video_freeze_algo)
    {
        log_deinit(amg_video_freeze_algo->logger);
        free(amg_video_freeze_algo);
    }
}

