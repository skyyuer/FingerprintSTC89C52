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
#include "i2c.h"
#include "lcd.h"
sbit led1 = P2^0;//led D1灯
sbit led2 = P2^1;
sbit led3 = P2^2;
sbit led4 = P2^3;
sbit led5 = P2^4;
sbit led6 = P2^5;
//char xdata ma[200];
/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{
	Restart_Init();
	while(1)
	{
		led5 = 0;
		led6 = 0;
		if(FPCommMode.isWorkFlag == 1)
		{
			//EX0=0;//关闭INT0的中断允许。
			 led2 = 0;
			 FP_Process();
			 led2 = 1;
			 FPCommMode.isWorkFlag =0;
			 FPCommMode.isSampleFlag = 0;
			 //EX0=1;//打开INT0的中断允许。
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

void Int1() interrupt 2
{
	System_Dly(1000);
	if(P3^3==0)
	{
		FPCommMode.isSampleFlag = 1;
		FPCommMode.isWorkFlag = 1;
	}
}

