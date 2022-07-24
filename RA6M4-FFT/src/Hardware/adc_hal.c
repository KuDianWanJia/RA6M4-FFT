#include "adc_scan_periodic.h"
#include "adc_hal.h"
#include "dtc_hal.h"

/*user buffer */
//ADC0  -  A、B两组  -  每组两个通道  -   每个通道Ping Pong双缓存  -  每个缓存大小
uint16_t g_buffer_adc[NUM_SAMPLES_PER_CHANNEL];

/*flags to capture adc 0/1 events*/
volatile bool g_adc0_group_a_flag = false ;
/* flag to capture error event*/
volatile bool g_err_flag_adc0 = false;

/*transfer info for ADC unit 0/1 and group A */
/*  DTC数据传输使用
transfer_info_t g_transfer_adc_group_a[] =
{    //通道零
     [ZERO] =
     {
      .dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
      .repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
      .irq = TRANSFER_IRQ_END,
      .chain_mode = TRANSFER_CHAIN_MODE_EACH,
      .src_addr_mode = TRANSFER_ADDR_MODE_FIXED,             //源地址固定
      .size = TRANSFER_SIZE_2_BYTE,
      .mode = TRANSFER_MODE_NORMAL,
      .p_dest = (void*) &g_buffer_adc[ZERO], //目标地址指针
      .p_src = (void const*) &R_ADC0->ADDR[ZERO],            //源地址指针
      .num_blocks = ZERO,
      .length = NUM_SAMPLES_PER_CHANNEL,                     //缓存长度
     },
};
*/

/*****************************************************************************************************************
 * @brief       Initializes ADC module instances.
 * @param[IN]   p_ctrl_adc         ADC instance control structure.
 * @param[IN]   p_cfg_adc          ADC instance config  structure.
 * @retval      FSP_SUCCESS        ADC instance opened successfully.
 * @retval      Any Other Error code apart from FSP_SUCCES on Unsuccessful opening of adc instance .
 ****************************************************************************************************************/
fsp_err_t init_hal_adc(adc_ctrl_t * p_ctrl_adc, adc_cfg_t const * const p_cfg_adc)
{
    fsp_err_t err = FSP_SUCCESS;
    /*open adc module */
    err = R_ADC_Open(p_ctrl_adc, p_cfg_adc);
    if(FSP_SUCCESS != err) { ; }
    return err;
}

/*****************************************************************************************************************
 * @brief       Configures the ADC channel specific settings.
 * @param[IN]   p_ctrl_adc                 ADC instance control structure.
 * @param[IN]   p_channel_cfg_adc          ADC instance  configured channel structure.
 * @retval      FSP_SUCCESS                On successful adc channel configuration setting..
 * @retval      Any Other Error code apart from FSP_SUCCES on Unsuccessful channel configuring of adc instance.
 ****************************************************************************************************************/
fsp_err_t adc_channel_config(adc_ctrl_t * p_ctrl_adc, void const * const p_channel_cfg_adc)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Configure ADC channel specific settings */
    err = R_ADC_ScanCfg(p_ctrl_adc, p_channel_cfg_adc);
    if(FSP_SUCCESS != err) { ; }
    return err;
}

/*******************************************************************************************************************//**
 * @brief       This functions closes ADC instance.
 * @param[IN]   p_ctrl_adc                  ADC control instance
 * @retval      FSP_SUCCESS                 Upon successful close operation.
 * @retval      Any Other Error code apart from FSP_SUCCESS
 **********************************************************************************************************************/
void deinit_hal_adc(adc_ctrl_t * p_ctrl_adc)
{
    fsp_err_t err = FSP_SUCCESS;
    /*Close opened ADC instances */
    err = R_ADC_Close(p_ctrl_adc);
    if(FSP_SUCCESS != err) { ; }
}

/*******************************************************************************************************************//**
 * @brief       This functions start adc scan operation.
 * @param[IN]   p_ctrl_adc                  ADC control instance
 * @retval      FSP_SUCCESS                 Upon successful scan start operation.
 * @retval      Any Other Error code apart from FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t scan_start_adc(adc_ctrl_t * p_ctrl_adc)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Scan start adc, API will return success but will start scanning once trigger event are received. */
    err = R_ADC_ScanStart(p_ctrl_adc);
    if(FSP_SUCCESS != err) { ; }
    return err;
}

/*******************************************************************************************************************//**
 * @brief      This function reads the adc output data from the prescribed channel and checks adc status
 * @param[IN]  p_ctrl_adc：ADC control instance     *buffer：DATA Bufffer
 * @retval     FSP_SUCCESS                  Upon successful stops the adc scan and closes the adc
 * @retval     Any Other Error code apart from FSP_SUCCES  Unsuccessful stop or close of adc
 ***********************************************************************************************************************/
fsp_err_t adc_read_data(adc_ctrl_t * p_ctrl_adc, uint16_t *buffer)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status

    /* Read the result */
    err = R_ADC_Read(p_ctrl_adc, ADC_CHANNEL_0, buffer);

    /* handle error */
    if (FSP_SUCCESS != err)
    {
        /* ADC Failure message */
        return err;
    }
    return err;
}

extern uint16_t DMAF;
/*******************************************************************************************************************//**
 *  @brief        User defined callback function for ADC unit 0
 *  @param[IN]    p_args
 *  @retval       None
 **********************************************************************************************************************/
void g_adc0_callback(adc_callback_args_t *p_args)
{
    static uint32_t i = 0;
    adc_event_t event = p_args->event;

    switch(event)
    {
        case ADC_EVENT_SCAN_COMPLETE:  //ADC采集完成中断事件
        {
            /*set the flag for adc 0 group A, when ADC_EVENT_SCAN_COMPLETE_GROUP_A occured */
            g_adc0_group_a_flag = true;
            adc_read_data(&g_adc0_ctrl, &g_buffer_adc[i]);
            i++;
            if(i >= 128)    //获取128次AD采集后，置位TFT运算和OLED刷新标志
            {
                DMAF = 1;
                i = 0;
            }
        }
        break;

        default:
        {
            /* set the err flag for adc 0, when received event is apart from group A/B scan complete event*/
            g_err_flag_adc0 = true;
        }
        break;
    }
}
