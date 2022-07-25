/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-10-10     Sherman       first version
 * 2021-11-03     Sherman       Add icu_sample
 */

#include <rtthread.h>
#include "hal_data.h"
#include <rtdevice.h>

#include "OLED_GFX.h"
#include "OLED_Driver.h"
#include "OLED_Animation.h"

#include "NvHai.h"
#include "ShaoNv.h"
#include "XingKong.h"
#include "YinYangShi.h"

#include "FFT.h"

#include "dtc_hal.h"
#include "adc_hal.h"
#include "gpt_hal.h"
#include "elc_hal.h"
#include "adc_scan_periodic.h"

void GPIO_Init_OLED(void);
/* Private Function Declaration */
static void general_signal_acquisition_init (void);
static void handle_error(fsp_err_t err, char *err_str, module_name_t module);

uint16_t Flag = 0;

/*extern variables */
extern uint16_t g_buffer_adc[];
extern volatile bool g_adc0_group_a_flag;
extern volatile bool g_err_flag_adc0;
//extern transfer_info_t g_transfer_adc_group_a[];

static rt_thread_t  OLED_Thread;

void OLED_Task(void *parameter)    //2ms执行一次
{
    while (1)
    {
        //动画显示
        /*
        Clear_Screen();
        GRAM_ShowImage(0,0,127,127,gImage_xingkong);
        Motion_Mind();
        Refrash_Screen();
        */
        //频谱显示
        if(Flag == 1)  //如果128点时域实数样本传输完成
        {
            Flag = 0;
            bit_reversed();                      //128点时域实数样本转复数并倒位序存储
            FFT();                               //FFT运算
            Quantization_of_FFT_results();       //求FFT结果的幅值谱及其幅值谱的量化

            Clear_Screen();
            GRAM_ShowImage(0,0,127,127,gImage_xingkong);
            Motion_Mind();
            Display_refresh();
            Refrash_Screen();
        }
        rt_thread_mdelay(2);
    }
}

void hal_entry(void)
{
    fsp_err_t err= FSP_SUCCESS;

    rt_kprintf("\nHello RT-Thread!\n");

    /***********************************************/
    /* Initialize the hal driver's for signal acquisition */
    general_signal_acquisition_init();

    /* start ADC Unit 0*/
    err = scan_start_adc(&g_adc0_ctrl);
    handle_error(err,"\r\n** start_adc for Unit 0 FAILED ** \r\n", ALL);

    /*start gpt timer*/
    err = gpt_timer_start(&g_timer0_ctrl);
    handle_error(err,"\r\n** start gpt timer 0 FAILED ** \r\n", ALL);
    /***********************************************/

    //OLED相关初始化
    GPIO_Init_OLED();

    R_PORT1->PODR = 0xff;
    R_PORT4->PODR_b.PODR11 = 1;
    R_PORT4->PODR_b.PODR13 = 1;
    R_PORT4->PODR_b.PODR9 = 1;
    R_PORT4->PODR_b.PODR8 = 1;
    R_PORT4->PODR_b.PODR7 = 1;
    //频谱相关初始化
    FFT_Init();
    //OLED相关初始化
    Device_Init();
    Motion_MindInit();

    OLED_Thread = rt_thread_create("Loop2MS",OLED_Task,RT_NULL,1024,2,1);
    if(OLED_Thread != RT_NULL) rt_thread_startup(OLED_Thread);
}

/*******************************************************************************************************************//**
 * @brief       This functions initializes and enables adc, gpt, dtc and elc modules to be used as signal acquisition module.
 * @param[IN]   None
 * @retval      None
 * @retval      Any Other Error code apart from FSP_SUCCESS  Unsuccessful open.
 **********************************************************************************************************************/
static void general_signal_acquisition_init (void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Initialize all the links in the Event Link Controller */
    err = init_hal_elc(&g_elc_ctrl,&g_elc_cfg);
    if(FSP_SUCCESS != err) { ; }

    /* Initialize DTC instance and reconfigure in chain mode for instance unit 0, group a */
    //err = init_hal_dtc(&g_transfer_adc0_group_a_ctrl, &g_transfer_adc0_group_a_cfg);
    //handle_error(err,"\r\n** dtc_init for unit 0, group a failed ** \r\n", ELC_MODULE);

    //err = dtc_hal_reconfigure(&g_transfer_adc0_group_a_ctrl,  &g_transfer_adc_group_a[ZERO]);
    //handle_error(err,"\r\n** dtc reconfiguration for unit 0, group a failed ** \r\n", ELC_DTC1);

    /* Initialize ADC Unit 0 and configure channels for it*/
    err = init_hal_adc(&g_adc0_ctrl,&g_adc0_cfg);
    handle_error(err,"\r\n** adc_init for unit 0 failed ** \r\n", ELC_DTC_MODULE_ALL);

    err = adc_channel_config(&g_adc0_ctrl, &g_adc0_channel_cfg);
    handle_error(err,"\r\n** adc_channel_config for unit 0 failed ** \r\n", ELC_DTC_ADC0_MODULE);

    /* Initialize GPT timer 0*/
    err = init_hal_gpt(&g_timer0_ctrl, &g_timer0_cfg);
    handle_error(err,"\r\n** gpt_init for timer 0 failed ** \r\n", ELC_DTC_ADC_MODULE_ALL);

    /* Enable the operation of the Event Link Controller */
    err = elc_enable(&g_elc_ctrl);
    handle_error(err,"\r\n** R_ELC_Enable failed ** \r\n", ALL);

    /*Enable transfers for adc unit 0 group a*/
    //err = dtc_enable(&g_transfer_adc0_group_a_ctrl);
    //handle_error(err,"\r\n** dtc_enable for ADC unit 0 group a failed ** \r\n", ALL);
}

/*******************************************************************************************************************//**
 *  @brief       Closes the ELC, DTC, GPT and ADC module, Print and traps error.
 *  @param[IN]   status    error status
 *  @param[IN]   err_str   error string
 *  @param[IN]   module    module to be closed
 *  @retval      None
 **********************************************************************************************************************/
static void handle_error( fsp_err_t err, char *err_str, module_name_t module)
{
    if(FSP_SUCCESS != err)
    {
        switch (module)
        {
            case ELC_MODULE:
            {
                /* close elc instance */
                deinit_hal_elc(&g_elc_ctrl);
            }
            break;
            case ELC_DTC1:
            {
                /* close elc instance */
                deinit_hal_elc(&g_elc_ctrl);

                /* close dtc instance */
                deinit_hal_dtc(&g_transfer_adc0_group_a_ctrl);
            }
            break;
            case ELC_DTC12:
            {
                /* close elc instance */
                deinit_hal_elc(&g_elc_ctrl);

                /* close dtc instances */
                deinit_hal_dtc(&g_transfer_adc0_group_a_ctrl);
            }
            break;

            case ELC_DTC_ADC0_MODULE:
            {
                /* close elc instance */
                deinit_hal_elc(&g_elc_ctrl);

                /* close dtc instances */
                deinit_hal_dtc(&g_transfer_adc0_group_a_ctrl);

                /* close adc instance */
                deinit_hal_adc(&g_adc0_ctrl);
            }
            break;
            case ELC_DTC_ADC_MODULE_ALL:
            {
                /* close elc instance */
                deinit_hal_elc(&g_elc_ctrl);

                /* close DTC opened instance */
                deinit_hal_dtc(&g_transfer_adc0_group_a_ctrl);

                /* close adc instance */
                deinit_hal_adc(&g_adc0_ctrl);
            }
            break;
            case ELC_DTC_ADC_GPT0_MODULE:
            {
                /* close elc instance */
                deinit_hal_elc(&g_elc_ctrl);

                /* close DTC instances */
                deinit_hal_dtc(&g_transfer_adc0_group_a_ctrl);

                /*close adc instances*/
                deinit_hal_adc(&g_adc0_ctrl);

                /* close gpt instance */
                deinit_hal_gpt(&g_timer0_ctrl);
            }
            break;
            case ALL:
            {
                /* close elc instance */
                deinit_hal_elc(&g_elc_ctrl);

                /*close adc instances*/
                deinit_hal_adc(&g_adc0_ctrl);

                /* close GPT 0 and 1 instance */
                deinit_hal_gpt(&g_timer0_ctrl);

                /* close DTC opened instance */
                deinit_hal_dtc(&g_transfer_adc0_group_a_ctrl);
            }
            break;
            default:
            {
                /*do nothing */
            }
        }
    }
}

void GPIO_Init_OLED(void)
{
    R_PMISC->PWPR_b.B0WI = 0;   /* 解锁 PmnPFS */
    R_PMISC->PWPR_b.PFSWE = 1;

    R_PFS->PORT[1].PIN[0].PmnPFS_b.PMR = 0;   /* 普通IO */
    R_PFS->PORT[1].PIN[0].PmnPFS_b.PDR = 1;   /* 输出 */
    R_PFS->PORT[1].PIN[0].PmnPFS_b.PCR = 1;   /* 使能上拉*/
    R_PFS->PORT[1].PIN[0].PmnPFS_b.NCODR = 0; /* CMOS模式 非开漏*/
    R_PFS->PORT[1].PIN[0].PmnPFS_b.DSCR = 3;  /* 高驱动能力 */
    R_PFS->PORT[1].PIN[0].PmnPFS_b.EOFR = 0;  /* 未使用事件模式所以写默认值 */
    R_PFS->PORT[1].PIN[0].PmnPFS_b.ISEL = 0;  /* 不使能外部中断 */
    R_PFS->PORT[1].PIN[0].PmnPFS_b.ASEL = 0;  /* 非AD */
    R_PFS->PORT[1].PIN[0].PmnPFS_b.PODR = 0;  /* 默认输出0 熄灯*/
    R_PFS->PORT[1].PIN[0].PmnPFS_b.PSEL = 0;  /* PMR=1时才有效 */

    R_PFS->PORT[1].PIN[1].PmnPFS_b.PMR = 0;   /* 普通IO */
    R_PFS->PORT[1].PIN[1].PmnPFS_b.PDR = 1;   /* 输出 */
    R_PFS->PORT[1].PIN[1].PmnPFS_b.PCR = 1;   /* 使能上拉*/
    R_PFS->PORT[1].PIN[1].PmnPFS_b.NCODR = 0; /* CMOS模式 非开漏*/
    R_PFS->PORT[1].PIN[1].PmnPFS_b.DSCR = 3;  /* 高驱动能力 */
    R_PFS->PORT[1].PIN[1].PmnPFS_b.EOFR = 0;  /* 未使用事件模式所以写默认值 */
    R_PFS->PORT[1].PIN[1].PmnPFS_b.ISEL = 0;  /* 不使能外部中断 */
    R_PFS->PORT[1].PIN[1].PmnPFS_b.ASEL = 0;  /* 非AD */
    R_PFS->PORT[1].PIN[1].PmnPFS_b.PODR = 0;  /* 默认输出0 熄灯*/
    R_PFS->PORT[1].PIN[1].PmnPFS_b.PSEL = 0;  /* PMR=1时才有效 */

    R_PFS->PORT[1].PIN[2].PmnPFS_b.PMR = 0;   /* 普通IO */
    R_PFS->PORT[1].PIN[2].PmnPFS_b.PDR = 1;   /* 输出 */
    R_PFS->PORT[1].PIN[2].PmnPFS_b.PCR = 1;   /* 使能上拉*/
    R_PFS->PORT[1].PIN[2].PmnPFS_b.NCODR = 0; /* CMOS模式 非开漏*/
    R_PFS->PORT[1].PIN[2].PmnPFS_b.DSCR = 3;  /* 高驱动能力 */
    R_PFS->PORT[1].PIN[2].PmnPFS_b.EOFR = 0;  /* 未使用事件模式所以写默认值 */
    R_PFS->PORT[1].PIN[2].PmnPFS_b.ISEL = 0;  /* 不使能外部中断 */
    R_PFS->PORT[1].PIN[2].PmnPFS_b.ASEL = 0;  /* 非AD */
    R_PFS->PORT[1].PIN[2].PmnPFS_b.PODR = 0;  /* 默认输出0 熄灯*/
    R_PFS->PORT[1].PIN[2].PmnPFS_b.PSEL = 0;  /* PMR=1时才有效 */

    R_PFS->PORT[1].PIN[3].PmnPFS_b.PMR = 0;   /* 普通IO */
    R_PFS->PORT[1].PIN[3].PmnPFS_b.PDR = 1;   /* 输出 */
    R_PFS->PORT[1].PIN[3].PmnPFS_b.PCR = 1;   /* 使能上拉*/
    R_PFS->PORT[1].PIN[3].PmnPFS_b.NCODR = 0; /* CMOS模式 非开漏*/
    R_PFS->PORT[1].PIN[3].PmnPFS_b.DSCR = 3;  /* 高驱动能力 */
    R_PFS->PORT[1].PIN[3].PmnPFS_b.EOFR = 0;  /* 未使用事件模式所以写默认值 */
    R_PFS->PORT[1].PIN[3].PmnPFS_b.ISEL = 0;  /* 不使能外部中断 */
    R_PFS->PORT[1].PIN[3].PmnPFS_b.ASEL = 0;  /* 非AD */
    R_PFS->PORT[1].PIN[3].PmnPFS_b.PODR = 0;  /* 默认输出0 熄灯*/
    R_PFS->PORT[1].PIN[3].PmnPFS_b.PSEL = 0;  /* PMR=1时才有效 */

    R_PFS->PORT[1].PIN[4].PmnPFS_b.PMR = 0;   /* 普通IO */
    R_PFS->PORT[1].PIN[4].PmnPFS_b.PDR = 1;   /* 输出 */
    R_PFS->PORT[1].PIN[4].PmnPFS_b.PCR = 1;   /* 使能上拉*/
    R_PFS->PORT[1].PIN[4].PmnPFS_b.NCODR = 0; /* CMOS模式 非开漏*/
    R_PFS->PORT[1].PIN[4].PmnPFS_b.DSCR = 3;  /* 高驱动能力 */
    R_PFS->PORT[1].PIN[4].PmnPFS_b.EOFR = 0;  /* 未使用事件模式所以写默认值 */
    R_PFS->PORT[1].PIN[4].PmnPFS_b.ISEL = 0;  /* 不使能外部中断 */
    R_PFS->PORT[1].PIN[4].PmnPFS_b.ASEL = 0;  /* 非AD */
    R_PFS->PORT[1].PIN[4].PmnPFS_b.PODR = 0;  /* 默认输出0 熄灯*/
    R_PFS->PORT[1].PIN[4].PmnPFS_b.PSEL = 0;  /* PMR=1时才有效 */

    R_PFS->PORT[1].PIN[5].PmnPFS_b.PMR = 0;   /* 普通IO */
    R_PFS->PORT[1].PIN[5].PmnPFS_b.PDR = 1;   /* 输出 */
    R_PFS->PORT[1].PIN[5].PmnPFS_b.PCR = 1;   /* 使能上拉*/
    R_PFS->PORT[1].PIN[5].PmnPFS_b.NCODR = 0; /* CMOS模式 非开漏*/
    R_PFS->PORT[1].PIN[5].PmnPFS_b.DSCR = 3;  /* 高驱动能力 */
    R_PFS->PORT[1].PIN[5].PmnPFS_b.EOFR = 0;  /* 未使用事件模式所以写默认值 */
    R_PFS->PORT[1].PIN[5].PmnPFS_b.ISEL = 0;  /* 不使能外部中断 */
    R_PFS->PORT[1].PIN[5].PmnPFS_b.ASEL = 0;  /* 非AD */
    R_PFS->PORT[1].PIN[5].PmnPFS_b.PODR = 0;  /* 默认输出0 熄灯*/
    R_PFS->PORT[1].PIN[5].PmnPFS_b.PSEL = 0;  /* PMR=1时才有效 */

    R_PFS->PORT[1].PIN[6].PmnPFS_b.PMR = 0;   /* 普通IO */
    R_PFS->PORT[1].PIN[6].PmnPFS_b.PDR = 1;   /* 输出 */
    R_PFS->PORT[1].PIN[6].PmnPFS_b.PCR = 1;   /* 使能上拉*/
    R_PFS->PORT[1].PIN[6].PmnPFS_b.NCODR = 0; /* CMOS模式 非开漏*/
    R_PFS->PORT[1].PIN[6].PmnPFS_b.DSCR = 3;  /* 高驱动能力 */
    R_PFS->PORT[1].PIN[6].PmnPFS_b.EOFR = 0;  /* 未使用事件模式所以写默认值 */
    R_PFS->PORT[1].PIN[6].PmnPFS_b.ISEL = 0;  /* 不使能外部中断 */
    R_PFS->PORT[1].PIN[6].PmnPFS_b.ASEL = 0;  /* 非AD */
    R_PFS->PORT[1].PIN[6].PmnPFS_b.PODR = 0;  /* 默认输出0 熄灯*/
    R_PFS->PORT[1].PIN[6].PmnPFS_b.PSEL = 0;  /* PMR=1时才有效 */

    R_PFS->PORT[1].PIN[7].PmnPFS_b.PMR = 0;   /* 普通IO */
    R_PFS->PORT[1].PIN[7].PmnPFS_b.PDR = 1;   /* 输出 */
    R_PFS->PORT[1].PIN[7].PmnPFS_b.PCR = 1;   /* 使能上拉*/
    R_PFS->PORT[1].PIN[7].PmnPFS_b.NCODR = 0; /* CMOS模式 非开漏*/
    R_PFS->PORT[1].PIN[7].PmnPFS_b.DSCR = 3;  /* 高驱动能力 */
    R_PFS->PORT[1].PIN[7].PmnPFS_b.EOFR = 0;  /* 未使用事件模式所以写默认值 */
    R_PFS->PORT[1].PIN[7].PmnPFS_b.ISEL = 0;  /* 不使能外部中断 */
    R_PFS->PORT[1].PIN[7].PmnPFS_b.ASEL = 0;  /* 非AD */
    R_PFS->PORT[1].PIN[7].PmnPFS_b.PODR = 0;  /* 默认输出0 熄灯*/
    R_PFS->PORT[1].PIN[7].PmnPFS_b.PSEL = 0;  /* PMR=1时才有效 */

    R_PFS->PORT[4].PIN[3].PmnPFS_b.PMR = 0;   /* 普通IO */
    R_PFS->PORT[4].PIN[3].PmnPFS_b.PDR = 1;   /* 输出 */
    R_PFS->PORT[4].PIN[3].PmnPFS_b.PCR = 1;   /* 使能上拉*/
    R_PFS->PORT[4].PIN[3].PmnPFS_b.NCODR = 0; /* CMOS模式 非开漏*/
    R_PFS->PORT[4].PIN[3].PmnPFS_b.DSCR = 3;  /* 高驱动能力 */
    R_PFS->PORT[4].PIN[3].PmnPFS_b.EOFR = 0;  /* 未使用事件模式所以写默认值 */
    R_PFS->PORT[4].PIN[3].PmnPFS_b.ISEL = 0;  /* 不使能外部中断 */
    R_PFS->PORT[4].PIN[3].PmnPFS_b.ASEL = 0;  /* 非AD */
    R_PFS->PORT[4].PIN[3].PmnPFS_b.PODR = 0;  /* 默认输出0 熄灯*/
    R_PFS->PORT[4].PIN[3].PmnPFS_b.PSEL = 0;  /* PMR=1时才有效 */

    R_PFS->PORT[4].PIN[4].PmnPFS_b.PMR = 0;   /* 普通IO */
    R_PFS->PORT[4].PIN[4].PmnPFS_b.PDR = 1;   /* 输出 */
    R_PFS->PORT[4].PIN[4].PmnPFS_b.PCR = 1;   /* 使能上拉*/
    R_PFS->PORT[4].PIN[4].PmnPFS_b.NCODR = 0; /* CMOS模式 非开漏*/
    R_PFS->PORT[4].PIN[4].PmnPFS_b.DSCR = 3;  /* 高驱动能力 */
    R_PFS->PORT[4].PIN[4].PmnPFS_b.EOFR = 0;  /* 未使用事件模式所以写默认值 */
    R_PFS->PORT[4].PIN[4].PmnPFS_b.ISEL = 0;  /* 不使能外部中断 */
    R_PFS->PORT[4].PIN[4].PmnPFS_b.ASEL = 0;  /* 非AD */
    R_PFS->PORT[4].PIN[4].PmnPFS_b.PODR = 0;  /* 默认输出0 熄灯*/
    R_PFS->PORT[4].PIN[4].PmnPFS_b.PSEL = 0;  /* PMR=1时才有效 */

    R_PFS->PORT[4].PIN[5].PmnPFS_b.PMR = 0;   /* 普通IO */
    R_PFS->PORT[4].PIN[5].PmnPFS_b.PDR = 1;   /* 输出 */
    R_PFS->PORT[4].PIN[5].PmnPFS_b.PCR = 1;   /* 使能上拉*/
    R_PFS->PORT[4].PIN[5].PmnPFS_b.NCODR = 0; /* CMOS模式 非开漏*/
    R_PFS->PORT[4].PIN[5].PmnPFS_b.DSCR = 3;  /* 高驱动能力 */
    R_PFS->PORT[4].PIN[5].PmnPFS_b.EOFR = 0;  /* 未使用事件模式所以写默认值 */
    R_PFS->PORT[4].PIN[5].PmnPFS_b.ISEL = 0;  /* 不使能外部中断 */
    R_PFS->PORT[4].PIN[5].PmnPFS_b.ASEL = 0;  /* 非AD */
    R_PFS->PORT[4].PIN[5].PmnPFS_b.PODR = 0;  /* 默认输出0 熄灯*/
    R_PFS->PORT[4].PIN[5].PmnPFS_b.PSEL = 0;  /* PMR=1时才有效 */

    R_PFS->PORT[4].PIN[6].PmnPFS_b.PMR = 0;   /* 普通IO */
    R_PFS->PORT[4].PIN[6].PmnPFS_b.PDR = 1;   /* 输出 */
    R_PFS->PORT[4].PIN[6].PmnPFS_b.PCR = 1;   /* 使能上拉*/
    R_PFS->PORT[4].PIN[6].PmnPFS_b.NCODR = 0; /* CMOS模式 非开漏*/
    R_PFS->PORT[4].PIN[6].PmnPFS_b.DSCR = 3;  /* 高驱动能力 */
    R_PFS->PORT[4].PIN[6].PmnPFS_b.EOFR = 0;  /* 未使用事件模式所以写默认值 */
    R_PFS->PORT[4].PIN[6].PmnPFS_b.ISEL = 0;  /* 不使能外部中断 */
    R_PFS->PORT[4].PIN[6].PmnPFS_b.ASEL = 0;  /* 非AD */
    R_PFS->PORT[4].PIN[6].PmnPFS_b.PODR = 0;  /* 默认输出0 熄灯*/
    R_PFS->PORT[4].PIN[6].PmnPFS_b.PSEL = 0;  /* PMR=1时才有效 */

    R_PFS->PORT[4].PIN[7].PmnPFS_b.PMR = 0;   /* 普通IO */
    R_PFS->PORT[4].PIN[7].PmnPFS_b.PDR = 1;   /* 输出 */
    R_PFS->PORT[4].PIN[7].PmnPFS_b.PCR = 1;   /* 使能上拉*/
    R_PFS->PORT[4].PIN[7].PmnPFS_b.NCODR = 0; /* CMOS模式 非开漏*/
    R_PFS->PORT[4].PIN[7].PmnPFS_b.DSCR = 3;  /* 高驱动能力 */
    R_PFS->PORT[4].PIN[7].PmnPFS_b.EOFR = 0;  /* 未使用事件模式所以写默认值 */
    R_PFS->PORT[4].PIN[7].PmnPFS_b.ISEL = 0;  /* 不使能外部中断 */
    R_PFS->PORT[4].PIN[7].PmnPFS_b.ASEL = 0;  /* 非AD */
    R_PFS->PORT[4].PIN[7].PmnPFS_b.PODR = 0;  /* 默认输出0 熄灯*/
    R_PFS->PORT[4].PIN[7].PmnPFS_b.PSEL = 0;  /* PMR=1时才有效 */

    R_PMISC->PWPR_b.PFSWE = 0;   /* 锁定 PmnPFS */
    R_PMISC->PWPR_b.B0WI = 1;
}

/*
void icu_sample(void)
{
    //init
    rt_uint32_t pin = rt_pin_get(USER_INPUT);
    rt_kprintf("\n pin number : 0x%04X \n", pin);
    rt_err_t err = rt_pin_attach_irq(pin, PIN_IRQ_MODE_RISING, irq_callback_test, RT_NULL);
    if(RT_EOK != err)
    {
        rt_kprintf("\n attach irq failed. \n");
    }
    err = rt_pin_irq_enable(pin, PIN_IRQ_ENABLE);
    if(RT_EOK != err)
    {
        rt_kprintf("\n enable irq failed. \n");
    }
}
MSH_CMD_EXPORT(icu_sample, icu sample);
*/

