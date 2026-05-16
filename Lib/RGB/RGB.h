#ifndef _RGB_H_
#define _RGB_H_

#include "main.h"


#define Red_on HAL_GPIO_WritePin(RGB_R_GPIO_Port,RGB_R_Pin,0)
#define Red_off HAL_GPIO_WritePin(RGB_R_GPIO_Port,RGB_R_Pin,1)
#define Green_on HAL_GPIO_WritePin(RGB_G_GPIO_Port,RGB_G_Pin,0)
#define Green_off HAL_GPIO_WritePin(RGB_G_GPIO_Port,RGB_G_Pin,1)
#define Blue_on HAL_GPIO_WritePin(RGB_B_GPIO_Port,RGB_B_Pin,0)
#define Blue_off HAL_GPIO_WritePin(RGB_B_GPIO_Port,RGB_B_Pin,1)



void RGB_Set(uint8_t r, uint8_t g, uint8_t b);

#endif
