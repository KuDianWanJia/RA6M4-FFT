#ifndef ADC_HAL_H_
#define ADC_HAL_H_

#include "hal_data.h"
#include "r_adc_api.h"

#define NUM_SAMPLES_PER_CHANNEL (128)
#define NUM_ADC_CHANNELS        (1)
#define NUM_SAMPLE_BUFFER       (1)
#define ZERO                    (0U)
#define ONE                     (1U)
#define TWO                     (2U)
#define THREE                   (3U)
#define FOUR                    (4U)   //ADC channel 4
#define FIVE                    (5U)   //ADC channel 5
#define SIX                     (6U)   //ADC channel 6
#define SEVEN                   (7U)   //ADC channel 7
#define NINE                    (9U)   //ADC channel 9
#define SIXETEEN                (16U)  //ADC channel 16
#define SEVENETEEN              (17U)  //ADC channel 17
#define EIGHTTEEN               (18U)  //ADC channel 18
#define NINETEEN                (19U)  //ADC channel 19
#define TWENTY                  (20U)  //ADC channel 20
#define TWENTY_ONE              (21U)  //ADC channel 21
#define TWENTY_TWO              (22U)  //ADC Channel 22

typedef enum
{
    ADC_UNIT_0 = 0,
    ADC_UNIT_1,
    ADC_UNIT_NUM
}adc_unit_num_t;

typedef enum
{
    ADC_GROUP_SCAN_A = 0,
    ADC_GROUP_SCAN_B,
    ADC_GROUP_SCAN_NUM
}adc_scan_group_t;

/* functions declaration */
fsp_err_t init_hal_adc(adc_ctrl_t * p_ctrl_adc, adc_cfg_t const * const p_cfg_adc);
fsp_err_t adc_channel_config(adc_ctrl_t * p_ctrl_adc, void const * const p_channel_cfg_adc);
fsp_err_t scan_start_adc(adc_ctrl_t * p_ctrl_adc);
void deinit_hal_adc(adc_ctrl_t * p_ctrl_adc);

#endif /* ADC_HAL_H_ */
