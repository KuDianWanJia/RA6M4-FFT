#ifndef ELC_HAL_H_
#define ELC_HAL_H_

#include "r_elc_api.h"

/*function declaration */
fsp_err_t init_hal_elc(elc_ctrl_t * const p_ctrl_instance_elc,  elc_cfg_t const * const p_cfg_instance_elc);
fsp_err_t elc_enable(elc_ctrl_t * const p_ctrl_instance_elc);
void deinit_hal_elc(elc_ctrl_t * const p_ctrl_instance_elc);

#endif /* ELC_HAL_H_ */
