/** @file amg_video_state_info_detect.c
 *  @brief Amagi Video State Machine
 *
 *  @author Viswanath Srikanth Bathina (viswanath.b@amagi.com)
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <amg_video_state_detect.h>

typedef struct __amg_video_state_info{
    int idx;
    int *threshold;
    int *state_transfer;
} amg_video_state_info;

typedef struct __amg_video_mchn{
    int                     state_cnt;
    int                     input_cnt;
    int                     cur_state;
    int                     old_input;
    int                     cur_input_repeat_cnt;
    int                     cur_state_input_cnt;
    int                     prev_state_input_cnt;
    amg_video_state_info    *amg_video_state_ptrs;
} amg_video_mchn;

void* AmgVideoStates_Init(int state_cnt, int input_cnt)
{
    amg_video_mchn *AmgVideoStates;
    AmgVideoStates = (amg_video_mchn * ) malloc(sizeof(amg_video_mchn));
    AmgVideoStates->state_cnt = state_cnt;
    AmgVideoStates->input_cnt = input_cnt;
    AmgVideoStates->amg_video_state_ptrs = (amg_video_state_info *) malloc(sizeof(amg_video_state_info) * (uint64_t)state_cnt);
    
    int i; 
    for (i = 0 ; i<AmgVideoStates->state_cnt ; i++)
    {
        AmgVideoStates->amg_video_state_ptrs[i].threshold=(int*)malloc(sizeof(int)*(uint64_t)input_cnt);
        AmgVideoStates->amg_video_state_ptrs[i].state_transfer=(int*)malloc(sizeof(int)*(uint64_t)input_cnt);
    }
    AmgVideoStates->cur_state = 0;
    AmgVideoStates->old_input = 0;
    AmgVideoStates->cur_input_repeat_cnt = 0;
    AmgVideoStates->cur_state_input_cnt = 0;
    AmgVideoStates->prev_state_input_cnt = 0;
    return (void*)AmgVideoStates;
}

void AmgVideoStates_Add(void* handle,int idx, int *threshold, int *state_transfer)
{
    amg_video_mchn *AmgVideoStates = (amg_video_mchn*)handle;
    AmgVideoStates->amg_video_state_ptrs[idx].idx = idx;
    int i;
    for (i = 0 ; i<AmgVideoStates->input_cnt ; i++)
    {
        AmgVideoStates->amg_video_state_ptrs[idx].threshold[i] = threshold[i];
	    AmgVideoStates->amg_video_state_ptrs[idx].state_transfer[i] = state_transfer[i];
    }
}

void AmgVideoStates_Input(void* handle,int input)
{
    amg_video_mchn *AmgVideoStates = (amg_video_mchn*)handle;
    if (AmgVideoStates->old_input == input)
    {
        AmgVideoStates->cur_input_repeat_cnt++;
    }
    else 
    {
        AmgVideoStates->cur_input_repeat_cnt = 1;
        AmgVideoStates->old_input = input;
    }
    AmgVideoStates->cur_state_input_cnt++;
	if (AmgVideoStates->cur_input_repeat_cnt > AmgVideoStates->amg_video_state_ptrs[AmgVideoStates->cur_state].threshold[input])
    {
        if (AmgVideoStates->cur_state != AmgVideoStates->amg_video_state_ptrs[AmgVideoStates->cur_state].state_transfer[input])
        {
            AmgVideoStates->prev_state_input_cnt = AmgVideoStates->cur_state_input_cnt - AmgVideoStates->cur_input_repeat_cnt; 
            AmgVideoStates->cur_state_input_cnt = AmgVideoStates->cur_input_repeat_cnt;
        }
        AmgVideoStates->cur_state = AmgVideoStates->amg_video_state_ptrs[AmgVideoStates->cur_state].state_transfer[input];
        AmgVideoStates->cur_input_repeat_cnt = 1;
	}
}

int AmgVideoStates_GetState(void* handle)
{
    amg_video_mchn *AmgVideoStates = (amg_video_mchn*)handle;
    return AmgVideoStates->cur_state;
}

int AmgVideoStates_GetCurStateCount(void* handle)
{
    amg_video_mchn *AmgVideoStates = (amg_video_mchn*)handle;
    return AmgVideoStates->cur_state_input_cnt;
}

int AmgVideoStates_GetPrevStateCount(void* handle)
{
    amg_video_mchn *AmgVideoStates = (amg_video_mchn*)handle;
    return AmgVideoStates->prev_state_input_cnt;
}

void AmgVideoStates_DeInit(void* handle)
{
    amg_video_mchn *AmgVideoStates = (amg_video_mchn*)handle;
    int i;
    for (i = 0 ; i<AmgVideoStates->state_cnt ; i++)
    {
        if (AmgVideoStates )
        {
            if (AmgVideoStates->amg_video_state_ptrs != NULL)
            {
                free(AmgVideoStates->amg_video_state_ptrs[i].threshold);
                free(AmgVideoStates->amg_video_state_ptrs[i].state_transfer);
            }
        }
    }
    if (AmgVideoStates )
        free(AmgVideoStates->amg_video_state_ptrs);
    free(AmgVideoStates);
}
void AmgVideoStates_Reset(void* handle)
{
    amg_video_mchn *AmgVideoStates = (amg_video_mchn*)handle;
    AmgVideoStates->cur_state = 0;
    AmgVideoStates->old_input = 0;
    AmgVideoStates->cur_input_repeat_cnt = 0;
    AmgVideoStates->cur_state_input_cnt = 0;
    AmgVideoStates->prev_state_input_cnt = 0;
}
