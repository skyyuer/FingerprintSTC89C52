/**************************************************************************************
*		              本文件用于51单片机的指纹锁												  *
实现现象：下载程序后D1指示灯点亮
注意事项：无																				  
***************************************************************************************/


#include "reg52.h"    //此文件中定义了单片机的一些特殊功能寄存器
#include "common.h"
#include "main.h"
#include "fp_module.h"
#include "uart.h"
#include "drive.h"


sbit led1 = P2^0;//led D1灯
sbit led2 = P2^1;
sbit led3 = P2^2;

/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/

unsigned char flag = 0;
unsigned char sign = 0;

void main()
{
	Restart_Init();
	P0 = 0x7f;
	while(1)
	{
		if(FPCommMode.isWorkFlag == 1)
		{
			 led2 = 0;
			 FP_Process();
			 led2 = 1;
			 FPCommMode.isWorkFlag =0;
		}
	}
}




void Int0()	interrupt 0		//外部中断0的中断函数
{
	System_Dly(1000);	 //延时消抖
	led1 = ~led1;
	if(P3^2==1)
	{
		FPCommMode.isWorkFlag = 1;
	}
}


