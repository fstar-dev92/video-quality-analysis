/** @file amg_video_state_detect.h
 *  @brief Amagi Video State Machine
 *
 *  @author Viswanath Srikanth Bathina (viswanath.b@amagi.com)
 */
#ifndef AMG_VIDEO_STATE_DETECT
#define AMG_VIDEO_STATE_DETECT

#ifdef __cplusplus
extern "C" {
#endif


/** @brief Initialize amg_video_state.
 *
 * This will initialize the amg_video_states context.
 *
 * @param state_cnt gives total number of states.
 *
 * @param input_cnt gives total number of inputs obtained to change states.
 *
 */
void* AmgVideoStates_Init(int state_cnt, int input_cnt);

/** @brief Addition of states.
 * 
 * This function will add states to the state machine.
 * 
 * @param threshold holds thresholds of the current state for different inputs
 *
 * @param state_transfer contains which state will the current state change to after reaching threshold
 */

void AmgVideoStates_Add(void* handle,int idx, int *threshold, int *state_transfer);

/** @brief input 
 *
 * This function takes input from the user and changes the state accordingly
 *
 * @param input shows which input is given as input
 *
 */
void AmgVideoStates_Input(void* handle,int input);

/**@brief Get Current State
 * 
 * This gives state in which the machine is.
 *
 */
int AmgVideoStates_GetState(void* handle);

/**brief Get current State input count 
 * 
 * 
 * This gives number inputs obtained in current state
 * 
 */
int AmgVideoStates_GetCurStateCount(void* handle);

/**brief Get previous State input count 
 * 
 * 
 * This gives number inputs obtained in previous state
 * 
 */
int AmgVideoStates_GetPrevStateCount(void* handle);

/** @brief Deinitialize.
 *
 * This function is used to Deinitialize the AmgVideoStates context.
 */
void AmgVideoStates_DeInit(void* handle);

/** @brief Reset state information 
 *
 * @param handle pointer which holds handle for AmgVideoStates context.
 */
void AmgVideoStates_Reset(void *handle);
#ifdef __cplusplus
}
#endif

#endif //AMG_VIDEO_STATE_DETECT
