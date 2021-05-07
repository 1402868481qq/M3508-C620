/**************************************************************************
作者：栋次大次
没有特别幸运，请先特别努力
**************************************************************************/

#include "Communication.h"

uint8_t data_to_send[50];//ANO地面站发送数据缓冲

extern int32_t  angle_cnt;
extern double point_pidout;
//给匿名上位机传送数据
void ANO_DT_Send_Senser(void)
{
  u8 _cnt=0;
  vs16 _temp;
  u8 sum = 0;
  u8 i=0;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0x02;
  data_to_send[_cnt++]=0;

	_temp = M3508[0].Speed;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = M3508[0].PID.Goal_Speed;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = angle_cnt*0.01;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = M3508[0].CNT_ABS*0.01;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
//	
	
  data_to_send[3] = _cnt-4; 
  sum = 0;
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  data_to_send[_cnt++] = sum;
	
	  for(i = 0; i < _cnt; i++){
    	while(USART_GetFlagStatus(SEND_PORT, USART_FLAG_TC) == RESET);
        USART_SendData(SEND_PORT, data_to_send[i]);	//发送数据到串口1 
		}
}


//void NiMing_Report(u8 fun, u8 *data, u8 len){
//	u8 send_buf[32];
//	u8 i;
//	if(len > 28) return;	//最多28字节数据 
//	send_buf[len + 3] = 0;	//校验数置零
//	send_buf[0] = 0XAA;	//帧头
//	send_buf[1] = fun;	//功能字
//	send_buf[2] = len;	//数据长度
//	for(i = 0; i < len; i++) send_buf[3 + i] = data[i];			//复制数据
//	for(i = 0; i < len + 3; i++) send_buf[len + 3] += send_buf[i];	//计算校验和	
//	for(i = 0; i < len + 4; i++){
//		while(USART_GetFlagStatus(SEND_PORT, USART_FLAG_TC) == RESET);
//		USART_SendData(SEND_PORT, send_buf[i]);	//发送数据到串口1 
//	}
//}

//void Motor_Info_Send(void){
//	u8 tbuf[16];
//	char *p =(char*)&M3508[0].Speed;
//	tbuf[0]=*((char*)p+0);
//	tbuf[1]=*((char*)p+1);
//	tbuf[2]=*((char*)p+2);
//	tbuf[3]=*((char*)p+3);
//	
//	p=(char*)&M3508[0].PID.Goal_Speed;
//	tbuf[4]=*((char*)p+0);
//	tbuf[5]=*((char*)p+1);
//	tbuf[6]=*((char*)p+2);
//	tbuf[7]=*((char*)p+3);
//	
//	p=(char*)&M3508[1].Speed;
//	tbuf[8]=*((char*)p+0);
//	tbuf[9]=*((char*)p+1);
//	tbuf[10]=*((char*)p+2);
//	tbuf[11]=*((char*)p+3);
//	
//	p=(char*)&M3508[1].PID.Goal_Speed;
//	tbuf[12]=*((char*)p+0);
//	tbuf[13]=*((char*)p+1);
//	tbuf[14]=*((char*)p+2);
//	tbuf[15]=*((char*)p+3);
//	
//	NiMing_Report(0xAA,tbuf,16);
//	// printf("%d\r\n", (int32_t)(M3508.Speed));
//}


	
