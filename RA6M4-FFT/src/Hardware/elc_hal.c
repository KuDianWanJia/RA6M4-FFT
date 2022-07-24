#include "elc_hal.h"
#include "hal_data.h"

/*****************************************************************************************************************
 * @brief       Initializes ELC module instances.
 * @param[IN]   p_ctrl_instance_elc         ELC instance control structure.
 * @param[IN]   p_cfg_instance_elc          ELC instance config  structure.
 * @retval      FSP_SUCCESS                 ELC instance opened successfully.
 * @retval      Any Other Error code apart from FSP_SUCCES on Unsuccessful opening of elc instance .
*****************************************************************************************************************/
fsp_err_t init_hal_elc(elc_ctrl_t * const p_ctrl_instance_elc, elc_cfg_t const * const p_cfg_instance_elc)
{
    fsp_err_t err = FSP_SUCCESS;
    /*Open elc instance */
    err = R_ELC_Open(p_ctrl_instance_elc, p_cfg_instance_elc);
    if(FSP_SUCCESS != err) { ; }
    return err;
}

/*****************************************************************************************************************
 * @brief       Enable ELC module instances.
 * @param[IN]   p_ctrl_instance_elc         ELC instance control structure.
 * @retval      FSP_SUCCESS                 ELC instance enabled successfully.
 * @retval      Any Other Error code apart from FSP_SUCCES on Unsuccessful enable of elc instance.
*****************************************************************************************************************/
fsp_err_t elc_enable(elc_ctrl_t * const p_ctrl_instance_elc)
{
    fsp_err_t err = FSP_SUCCESS;
    /* Enable the operation of the Event Link Controller */
    err = R_ELC_Enable(p_ctrl_instance_elc);
    if(FSP_SUCCESS != err) { ; }
    return err;
}

/*******************************************************************************************************************//**
 * @brief       This functions closes ELC instance.
 * @param[IN]   p_ctrl_instance_elc         ELC control instance
 * @retval      FSP_SUCCESS                 Upon successful close operation.
 * @retval      Any Other Error code apart from FSP_SUCCESS
 **********************************************************************************************************************/
void deinit_hal_elc(elc_ctrl_t * const p_ctrl_instance_elc)
{
    //fsp_err_t err = FSP_SUCCESS;
    /* close opened elc instance */
    //err = R_ELC_Close(p_ctrl_instance_elc);
    //if(FSP_SUCCESS != err) { ; }

    R_ELC_Close(p_ctrl_instance_elc);
}
