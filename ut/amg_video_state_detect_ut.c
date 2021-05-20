/** @file amg_video_state_detect_ut.c
 *  @brief Amagi Video State Machine
 *
 *  @author Viswanath Srikanth Bathina (viswanath.b@amagi.com)
 */


#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <amg_video_state_detect.h>

int main(int argc, char *argv[])
{
    if (argc < 4 || argc != 2*atoi(argv[2])*atoi(argv[1]) + atoi(argv[3]) + 4)
    {
        printf("Usage: %s <state count> <input count> <no of inputs> <st_cnt * in_cnt threshold><st_cnt * in_cnt state_transfer> <no_input * input values>\n", argv[0]);
        exit(1);
    }
    
    int i,j;
    int state_cnt = atoi(argv[1]);
    int input_cnt = atoi(argv[2]);
    int threshold[state_cnt][input_cnt];
    int state_transfer[state_cnt][input_cnt];
    for(i=0;i<state_cnt;i++)
    {
        for(j=0;j<input_cnt;j++)
        {
            threshold[i][j]=atoi(argv[4+i*input_cnt+j]);
            printf("threshold from state %d to %d is %d\n", i, j, threshold[i][j]);
        }
    }
    for(i=0;i<state_cnt;i++)
    {
        for(j=0;j<input_cnt;j++)
        {
            state_transfer[i][j]=atoi(argv[4+input_cnt*state_cnt+i*input_cnt+j]);
        }
    }
    printf("State Machine with %d states and %d inputs is Initialized\n", state_cnt,input_cnt);
    int no_of_inputs=atoi(argv[3]);
    int inputs[no_of_inputs];
    for(i=0;i<no_of_inputs;i++)
        inputs[i]=atoi(argv[4+2*state_cnt*input_cnt+i]);

    void* handle=AmgVideoStates_Init(state_cnt,input_cnt);
    
    for(i=0;i<state_cnt;i++)
    {
        AmgVideoStates_Add(handle,i,threshold[i],state_transfer[i]);
    }
   int st,ed,prev_state,cur_state; 
    for(i=0;i<no_of_inputs;i++)
    {
        prev_state=AmgVideoStates_GetState(handle);
        AmgVideoStates_Input(handle,inputs[i]);
        printf("Current state after input %d is %d for %d\n",inputs[i],AmgVideoStates_GetState(handle),AmgVideoStates_GetCurStateCount(handle));
        cur_state=AmgVideoStates_GetState(handle);
        if(prev_state==1 && cur_state==0)printf("In the black state for %d\n",AmgVideoStates_GetPrevStateCount(handle));

    }
    if(AmgVideoStates_GetState(handle)==1)printf("In the black state for %d\n",AmgVideoStates_GetCurStateCount(handle));
    printf("\n");
    AmgVideoStates_DeInit(handle);
    return 0;
}
