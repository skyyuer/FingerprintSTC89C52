#include"fp_module.h"
#include "common.h"
#include "uart.h"
#include "drive.h"

#define FP_COMM_GET_IMAGE1_DELAY       160  //第一次获取图像要等久一点
#define FP_COMM_SEND_GET_IMAGE1_DELAY  6   //第一次获取图像发送延时，因为是循环发送
#define FP_COMM_RXD_X10MS_DELAY        50

char FP_Pack_Head[6] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF};  //包头
char FP_Get_Img[6] = {0x01,0x00,0x03,0x01,0x00,0x05};    //获取图像1
char FP_Search[11]= {0x01,0x0,0x08,0x04,0x01,0x00,0x00,0x00,0xC8,0x00,0xD6}; //搜索
char FP_Img_To_Buffer1[7]={0x01,0x0,0x04,0x02,0x01,0x0,0x08}; //生成char1


FPCommMode_t FPCommMode;


void FP_ModeReceiveOk(void)
{
	switch(FPCommMode.IndexBak)
	{
	case FPMODE_CMD_GET_IMAGE1:
		FPCommMode.Index = FPMODE_CMD_GEN_CHAR1;
		break;
		
	case FPMODE_CMD_GEN_CHAR1:
		FPCommMode.Index = FPMODE_CMD_SEARCH_ALL;
		break;
	case FPMODE_CMD_SEARCH_ALL:
		FP_SearchAllResultProc();
		break;

	default:
		FPCommMode.Index = FPMODE_HANDLE_INIT;
		break;
	}
}





void FP_ModeReceiveFail(char timeout)
{
	switch(FPCommMode.IndexBak)
	{
	case FPMODE_CMD_GET_IMAGE1:
		FPCommMode.Index = FPMODE_HANDLE_INIT;
		//TraceMsg("Get image1 fail. \r\n",1);
		break;
		
	case FPMODE_CMD_GEN_CHAR1:
		//TraceMsg("Generate char1 fail. \r\n",1);
		FPCommMode.Index = FPMODE_HANDLE_INIT;
		break;
		
	case FPMODE_CMD_SEARCH_ALL:
		FP_SearchAllResultProc();
		break;
		
	default:
		FPCommMode.Index = FPMODE_HANDLE_INIT;
		break;
	}

	if(TRUE == timeout)
	{
	
	}
}


void FP_CommModeTask(void)
{
	switch(FPCommMode.Index)
	{
	case FPMODE_HANDLE_INIT:
		FPCommMode.Index = FPMODE_CMD_GET_IMAGE1;
		break;
		
	case FPMODE_CMD_GET_IMAGE1:
		FPCommMode.result = RESULT_WAITING;
		FPCommMode.x10msDly = FP_COMM_GET_IMAGE1_DELAY;
		FPCommMode.IndexBak = FPMODE_CMD_GET_IMAGE1;
		FPCommMode.Index = FPMODE_CMD_RECEIVING;
		FP_SendConstCmd(FP_Get_Img, 6);
		break;
	
	case FPMODE_CMD_GEN_CHAR1:
		FPCommMode.result = RESULT_WAITING;
		FPCommMode.x10msDly = FP_COMM_RXD_X10MS_DELAY * 2;
		FPCommMode.IndexBak = FPMODE_CMD_GEN_CHAR1;
		FPCommMode.Index = FPMODE_CMD_RECEIVING;
		FP_SendConstCmd(FP_Img_To_Buffer1, 7);
		break;

	case FPMODE_CMD_SEARCH_ALL:
		FPCommMode.result = RESULT_WAITING;
		//FPCommMode.FP_id = FP_COMM_INVAID_ID;
		FPCommMode.x10msDly = FP_COMM_RXD_X10MS_DELAY * 2;
		FPCommMode.IndexBak = FPMODE_CMD_SEARCH_ALL;
		FPCommMode.Index = FPMODE_CMD_RECEIVING;
		FP_SendConstCmd(FP_Search, 11);
		break;
		
	case FPMODE_CMD_RECEIVING:
		if(RESULT_OK == FPCommMode.result)
		{
			FP_ModeReceiveOk();
		}
		else if(RESULT_FAIL == FPCommMode.result)
		{
			FP_ModeReceiveFail(FALSE);
		}
		else if(0 == FPCommMode.x10msDly) //接收超时
		{
			FP_ModeReceiveFail(TRUE);
		}
		break;

	case FPMODE_OPEN_DOOR:
		FPCommMode.Index = FPMODE_HANDLE_NULL;
		Lock_Switch();
		break;

	case FPMODE_SEARCH_FAIL:
		if(0 == FPCommMode.x10msDly)
		{
			FPCommMode.Index = FPMODE_HANDLE_INIT;
		}
		break;
		
	case FPMODE_HANDLE_NULL:
		break;
		
	default:
		FPCommMode.Index = FPMODE_HANDLE_INIT;
		break;
	}
}




void FP_SendConstCmd(char *txbuf, int len)
{
       char sendcmd[20] = {0};

	Utility_Strncpy(sendcmd,FP_Pack_Head, 6);
	Utility_Strncpy(sendcmd + 6 ,txbuf, len);

	Uart_Send(sendcmd,len+6);	

}


//搜索命令结果处理
void FP_SearchAllResultProc(void)
{
	int pageID, score;
	pageID = ((int)FPCommMode.rxdata[10] << 8) + FPCommMode.rxdata[11];
	score  = ((int)FPCommMode.rxdata[12] << 8) + FPCommMode.rxdata[13];
	//返回结果不为0而且得分大于60打印出来
	//FPCommMode.FP_id = (char)pageID;
	if((score >= 60) && (0 == FPCommMode.rxdata[7]) && (7 == FPCommMode.rxdata[8]))
	{
		FPCommMode.Index = FPMODE_OPEN_DOOR;
		FPCommMode.x10msDly = 0;
	}
	else
	{
		FPCommMode.Index = FPMODE_HANDLE_NULL;
		//FPCommMode.x1sDly = 100;
		//FPCommMode.xBellFailed10msDly =100;
	}
}



void Match_init()
{
	FPCommMode.Index = FPMODE_HANDLE_INIT;
}



void FP_Process()
{
	unsigned int repeat = 1000;
	 FP_UartDataInit();
	 Match_init();
	while(repeat > 0)
	{
		FP_CommModeTask();
		FP_UartRxdTask();
		repeat--;
		System_Dly(1000);
	}
}
