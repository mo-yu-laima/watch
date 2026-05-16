#include "RGB.h"
#include "cmsis_os2.h"



/**
* @brief 设置RGB颜色
*
* 根据传入的r, g, b值来设置RGB颜色。
*
* @param r 红色值，0表示关闭，1表示开启
* @param g 绿色值，0表示关闭，1表示开启
* @param b 蓝色值，0表示关闭，1表示开启
*
* 作者：继续mo鱼
*/
void RGB_Set(uint8_t r, uint8_t g, uint8_t b)
{
    if(r == 1) Red_on;
    if(r == 0) Red_off;
    if(g == 1) Green_on;
    if(g == 0) Green_off;
    if(b == 1) Blue_on;
    if(b == 0) Blue_off;

}



//void RGB_Task()
//{
//    while(1)
//    {
//        RGB_Set(1, 0, 0);
//        osDelay(1000);
//
//        RGB_Set(0, 1, 0);
//        osDelay(1000);
//
//        RGB_Set(0, 0, 1);
//        osDelay(1000);
//
//        RGB_Set(1, 1, 0);
//        osDelay(1000);
//
//        RGB_Set(1, 0, 1);
//        osDelay(1000);
//
//        RGB_Set(0, 1, 1);
//        osDelay(1000);
//
//        RGB_Set(1, 1, 1);
//        osDelay(1000);
//    }
//
//}
