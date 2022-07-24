#include "gpt_hal.h"
#include "hal_data.h"

/*****************************************************************************************************************
 * @brief       Initializes GPT module instances.
 * @param[IN]   p_ctrl         GPT instance control structure.
 * @param[IN]   p_cfg          GPT instance config  structure.
 * @retval      FSP_SUCCESS    GPT instance opened successfully.
 * @retval      Any Other Error code apart from FSP_SUCCES on Unsuccessful opening of gpt instance .
 ****************************************************************************************************************/
fsp_err_t init_hal_gpt(timer_ctrl_t * const p_ctrl, timer_cfg_t const * const p_cfg)
{
    fsp_err_t err = FSP_SUCCESS;
    /*open gpt instance */
    err = R_GPT_Open(p_ctrl, p_cfg);
    if(FSP_SUCCESS != err) { ; }
    return err;
}

/*******************************************************************************************************************//**
 * @brief       This functions closes GPT instance.
 * @param[IN]   p_ctrl_timer                GPT control instance
 * @retval      FSP_SUCCESS                 Upon successful close operation.
 * @retval      Any Other Error code apart from FSP_SUCCESS
 **********************************************************************************************************************/
void deinit_hal_gpt(timer_ctrl_t * const p_ctrl_timer )
{
    //fsp_err_t err = FSP_SUCCESS;
    /*Close opened GPT instances */
    //err = R_GPT_Close(p_ctrl_timer);
    //if(FSP_SUCCESS != err) { ; }

    R_GPT_Close(p_ctrl_timer);
}

/*******************************************************************************************************************//**
 * @brief       This functions starts GPT timer.
 * @param[IN]   p_ctrl_timer                GPT control instance
 * @retval      FSP_SUCCESS                 Upon successful start operation.
 * @retval      Any Other Error code apart from FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t gpt_timer_start(timer_ctrl_t * const p_ctrl_timer)
{
    fsp_err_t err = FSP_SUCCESS;
    /*Start gpt timer */
    err = R_GPT_Start(p_ctrl_timer);
    if(FSP_SUCCESS != err) { ; }
    return err;
}

/*******************************************************************************************************************//**
 *  @brief        User defined callback function for GPTimer
 *  @param[IN]    p_args
 *  @retval       None
 **********************************************************************************************************************/
void g_gpt0_callback(timer_callback_args_t *p_args)
{
    timer_event_t event = p_args->event;

    switch(event)
    {
        case TIMER_EVENT_CYCLE_END:  //溢出中断事件
        {
            ;
        }
        break;

        default:
        {
            ;
        }
        break;
    }
}
