#ifndef __DHT11_H__
#define __DHT11_H__

#include "main.h"


uint8_t DHT11_Read_Byte();
uint8_t DHT11_check_state();
void DHT11_Start();
void delay_us(uint32_t us);
uint8_t DHT11_ReadData (uint8_t *Hum_Int, uint8_t *Hum_Dec, uint8_t *Temp_Int, uint8_t *Temp_Dec);

#endif