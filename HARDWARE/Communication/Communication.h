/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/

#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "sys.h"
#include "usart.h"
#include "Motor_Control.h"


#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)      ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )
	

extern void Motor_Info_Send(void);
void ANO_DT_Send_Senser(void);
#define SEND_PORT USART1




#endif

