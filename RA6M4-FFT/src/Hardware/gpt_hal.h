#ifndef GPT_HAL_H_
#define GPT_HAL_H_

#include "r_timer_api.h"

/*function declaration */
fsp_err_t init_hal_gpt(timer_ctrl_t * const p_ctrl, timer_cfg_t const * const p_cfg);
fsp_err_t gpt_timer_start(timer_ctrl_t * const p_ctrl_timer);
void deinit_hal_gpt(timer_ctrl_t * const p_ctrl_timer );

#endif /* GPT_HAL_H_ */
