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

/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/

unsigned char flag = 0;


void main()
{
	Restart_Init();

	while(1)
	{

		System_Dly(50000);
			System_Dly(50000);
				System_Dly(50000);
		Uart_Test();
	}
	/*while(0)
	{
		if(FPCommMode.isWorkFlag == 1)
		{
			 FP_Process();
			 FPCommMode.isWorkFlag =0;
		}
	}*/		
}




void Int0()	interrupt 0		//外部中断0的中断函数
{
	System_Dly(1000);	 //延时消抖
	if(P3^2==1)
	{
		FPCommMode.isWorkFlag = 1;
	}
}


