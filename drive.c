#include <reg52.h>
#include "drive.h"
#include "fp_module.h"
#include "uart.h"
#include "i2c.h"


sbit led2 = P2^1;  //D2
sbit led3 = P2^2;  //D3
sbit led4 = P2^3;
sbit led5 = P2^4;
sbit led6 = P2^5;
sbit beep = P1^5;	
//sbit pin0 = P0^0;
//sbit pin1 = P1^1; 
/*void EnInt(char flag)
{
	if(flag == 1)
	{
		EA = 1;	  //打开总中断
	}
	else
	{
		EA =0;
	}
	return;
}

void EnInt_Ex0(char flag)
{
	if(flag == 1)
	{
		EX0 = 1;	  //打开外部中断0
	}
	else
	{
		EX0 =0;
	}
	return;
}

void EnInt_It0(char flag)
{//设置触发方式
	if(flag == 1)
	{
		IT0 = 1;
	}
	else
	{
		IT0 = 0;
	}
}
 */
void Lock_Switch()
{
	led5 = 1;
	led6 = 0;
	//P0^0 = 1;
	//P0^1 = 0;
	//pin0 = 1;
	//pin1 = 0;
	//System_Dly(50000);
	System_Dly(50000);
	System_Dly(50000);
	led5 = 0;
	led6 = 0;
	//P0^0=0;
	//P0^1=0;
	//pin0 = 0;
	//pin1 = 0;
	System_Dly(50000);
	System_Dly(50000);
	System_Dly(50000);
	System_Dly(50000);
	//System_Dly(50000);
	//System_Dly(50000);
	led5 = 0;
	led6 = 1;
	//P0^0=0;
	//P0^1=1;
	//pin0 = 0;
	//pin1 = 1;
	//System_Dly(50000);
	System_Dly(50000);
	System_Dly(50000);
	led5 = 0;
	led6 = 0;
	//P0^0=0;
	//P0^1=0;
	//pin0 = 0;
	//pin1 = 0;
}

void Int0Init()
{
	//设置INT0
	IT0=1;//跳变沿出发方式(下降沿)
	EX0=1;//打开INT0的中断允许。	
	EA=1;//打开总中断	
}

void Int1Init()
{
	IT1=1;
	EX1=1;
	EA=1;
}


void System_Dly(unsigned int count)//10000约为450ms
{
	while(count--);
}




void Timer0Init()
{
	TMOD|=0X01;//选择为定时器0模式，工作方式1，仅用TR0打开启动。

	TH0=0XFC;	//给定时器赋初值，定时1ms
	TL0=0X18;	
	ET0=1;//打开定时器0中断允许
	EA=1;//打开总中断
	TR0=1;//打开定时器			
}

void Restart_Init()
{
	//P0^0=0;
	//P0^1=0;
	//pin0 = 0;
	//pin1 = 0;
	P0 = 0x00;
	Timer0Init();
	Uart_Init_9600();
	//Uart_Init_115200();	
	Int0Init();
	Int1Init();
	FPCommMode.isWorkFlag = 0;
	FPCommMode.isSampleFlag = 0;
	FPCommMode.StoreNum = At24c02Read(1);
	if(FPCommMode.StoreNum >= 0x64)//100
	{
		 FPCommMode.StoreNum = 0;
		 At24c02Write(1,FPCommMode.StoreNum);
	}
}


void Timer0() interrupt 1
{
	TH0=0XFC;	//给定时器赋初值，定时1ms
	TL0=0X18;
	if(FPCommMode.x1msDly>0)
	{
		FPCommMode.x1msDly--;
	}
	if(FPCommMode.x1msDly_FP_Process>0)
	{
		FPCommMode.x1msDly_FP_Process--;
	}
	if(FPCommMode.BellOpenx1msDly>0)
	{
		FPCommMode.BellOpenx1msDly--;
	}	
}

void Bell_Open(int x1ms)
{
	FPCommMode.BellOpenx1msDly =  x1ms;
	while(FPCommMode.BellOpenx1msDly>0)
	{
		beep = ~beep;
		System_Dly(10);
	}
	beep = 1;

}





