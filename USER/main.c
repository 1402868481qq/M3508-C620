#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "can.h" 
#include "Motor_Control.h"
#include "tim.h" 
#include "FreeRTOS.h"	
#include "task.h"
#include "Communication.h"


//�������ȼ�
#define START_TASK_PRIO			1
//�����ջ��С	
#define START_STK_SIZE 			256  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define KEY_TASK_PRIO		2
//�����ջ��С	
#define KEY_STK_SIZE 		128  
//������
TaskHandle_t KeyTask_Handler;
//������
void key_task(void *pvParameters);

//�������ȼ�
#define TASK1_TASK_PRIO		3
//�����ջ��С	
#define TASK1_STK_SIZE 		128  
//������
TaskHandle_t Task1Task_Handler;
//������
void task1_task(void *pvParameters);

//�������ȼ�
#define PVM_TASK_PRIO		4
//�����ջ��С	
#define PVM_STK_SIZE 		128  
//������
TaskHandle_t PVMTask_Handler;
//������
void pvm_task(void *pvParameters);

#if PPM_control
//�������ȼ�
#define PPM_TASK_PRIO		5
//�����ջ��С	
#define PPM_STK_SIZE 		256  
//������
TaskHandle_t PPMTask_Handler;
//������
void ppm_task(void *pvParameters);
#endif

float kppp;
float kiii;
float kddd;

int32_t frist_angle = 0;
int point_angle = 0;
int32_t  angle_cnt;
double point_pidout;

int main(void)
{
	 delay_init();	    	 //��ʱ������ʼ��	  
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//�����ж����ȼ�����Ϊ��4     0~15λ��ռ���ȼ�  û�������ȼ�
	 uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	 LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	 LCD_Init();			   	//��ʼ��LCD	
	 KEY_Init();				  //������ʼ��		 	
   Control_Initialize(); 
   CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,2,CAN_Mode_Normal);//CAN��ͨģʽ��ʼ��, ������1MKbps 
	 TIM3_Int_Init(15, 36000-1);   // 8ms�ж�һ��
	 ppm_pidinit();
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    //�����жϲ�������
	//����KEY����
	xTaskCreate((TaskFunction_t )key_task,             
                (const char*    )"key_task",           
                (uint16_t       )KEY_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )KEY_TASK_PRIO,        
                (TaskHandle_t*  )&KeyTask_Handler);  
   //����TASK1����
  xTaskCreate((TaskFunction_t )task1_task,             
                (const char*    )"task1_task",           
                (uint16_t       )TASK1_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TASK1_TASK_PRIO,        
                (TaskHandle_t*  )&Task1Task_Handler);   							
	//�����ٶȻ�����			
 	 xTaskCreate((TaskFunction_t )pvm_task,             
                (const char*    )"pvm_task",           
                (uint16_t       )PVM_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )PVM_TASK_PRIO,        
                (TaskHandle_t*  )&PVMTask_Handler); 
 	#if PPM_control
	//�����ǶȻ�����	
	 xTaskCreate((TaskFunction_t )ppm_task,             
                (const char*    )"ppm_task",           
                (uint16_t       )PPM_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )PPM_TASK_PRIO,        
                (TaskHandle_t*  )&PPMTask_Handler); 
  #endif								
	vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}


//key������
void key_task(void *pvParameters)
{
	u8 key;
	while(1)
	{
		key=KEY_Scan(0);
		switch(key)
		{
			case WKUP_PRES:
       vTaskSuspend(Task1Task_Handler); //��������
				break;
			case KEY1_PRES:
       vTaskResume(PPMTask_Handler);	 //�ָ�����
			  break;
			case KEY0_PRES:
				vTaskSuspend(PPMTask_Handler); //��������
			  LED0=1;     //����
				break;
		}
		vTaskDelay(10);			//��ʱ10ms 
	}
}


//task1������
void task1_task(void *pvParameters)
{
	u8 task1_num=0;
	
//	POINT_COLOR = BLACK;

//	LCD_DrawRectangle(5,110,115,314); 	//��һ������	
//	LCD_DrawLine(5,130,115,130);		//����
	POINT_COLOR = MAGENTA;
	LCD_ShowString(6,111,110,16,16,"Task1 Run:000");
	while(1)
	{
		task1_num++;	//����ִ1�д�����1 ע��task1_num1�ӵ�255��ʱ������㣡��
//		LED0=!LED0;
    printf("����1�Ѿ�ִ�У�%d��\r\n",task1_num);
		LCD_ShowxNum(86,111,task1_num,3,16,0x80);	//��ʾ����ִ�д���
    vTaskDelay(1000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}


void pvm_task(void *pvParameters)
{ 
    while(1)
   {	  
//			Motor_Speed_Control(M3508[0].PID.Goal_Speed, 0);
////			Motor_Speed_Control(M3508[1].PID.Goal_Speed, 1);
////			Motor_Speed_Control(M3508[2].PID.Goal_Speed, 2);
////			Motor_Speed_Control(M3508[3].PID.Goal_Speed, 3);
////			Motor_Speed_Control(M3508[4].PID.Goal_Speed, 4);
////			Motor_Speed_Control(M3508[5].PID.Goal_Speed, 5);		  
//			CAN_Send_Control_Value();
//      ANO_DT_Send_Senser();
      vTaskDelay(10);  
   }

}

#if PPM_control
void ppm_task(void *pvParameters)
{ 
	 POINT_COLOR = MAGENTA;
	 LCD_ShowString(6,200,110,16,16,"CntBase:");
	 
		while(1)
	{	  
		frist_angle = M3508[0].Angle;                 //��ס��ʼ��е�Ƕ�
    LCD_ShowxNum(86,200,frist_angle,4,16,0x80);
		if(frist_angle != 0) 
		{			
			break;
		}
		vTaskDelay(10); 
	}
	 
					 while(1)
				 {	  
	        LED0=0;	
          PID_PPM_SetKp(kppp);
					PID_PPM_SetKi(kiii);
					PID_PPM_SetKd(kddd);
					angle_cnt = RM820R_Ang2Cnt(point_angle);            //����Ŀ��Ƕȵ�������
					point_pidout =  Motor_point_PID(angle_cnt,M3508[0].CNT_ABS);    //�ǶȻ�PID���
					Motor_Speed_Control((int32_t)point_pidout, 0);                  //�ٶȻ������0��ID�� 1
					CAN_Send_Control_Value();                                       //CAN���͵�����������
					ANO_DT_Send_Senser();                                           //��������վ
					vTaskDelay(10); 
				 } 
}
#endif


//	int res = 0;
// int main(void)
// {	 
//	u8 key;
//	u8 i=0,t=0;

//	u8 canbuf[8] = {2,2,2,2,2,2,2,2};

//	
//	delay_init();	    	 //��ʱ������ʼ��	  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
//	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
//	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
//	LCD_Init();			   	//��ʼ��LCD	
//	KEY_Init();				  //������ʼ��		 	
//  Control_Initialize(); 
////	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,2,CAN_Mode_LoopBack);//CAN��ʼ������ģʽ,������500Kbps    
//  CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,2,CAN_Mode_Normal);//CAN��ͨģʽ��ʼ��, ������1MKbps 
//	TIM3_Int_Init(15, 36000-1);   // 8ms�ж�һ��
// 	POINT_COLOR=RED;//��������Ϊ��ɫ 
//	LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
//	LCD_ShowString(60,70,200,16,16,"CAN TEST");	
//	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
//	LCD_ShowString(60,110,200,16,16,"2015/1/15");
//	LCD_ShowString(60,130,200,16,16,"LoopBack Mode");	 
//	LCD_ShowString(60,150,200,16,16,"KEY0:Send WK_UP:Mode");//��ʾ��ʾ��Ϣ		
//	POINT_COLOR=BLUE;//��������Ϊ��ɫ	  
//	LCD_ShowString(60,170,200,16,16,"Count:");			//��ʾ��ǰ����ֵ	
//	LCD_ShowString(60,190,200,16,16,"Send Data:");		//��ʾ���͵�����	
//	LCD_ShowString(60,250,200,16,16,"Receive Data:");	//��ʾ���յ�������		
// 	while(1)
//	{	   
//		key=KEY_Scan(0);
//		if(key==KEY0_PRES)//KEY0����,����һ������
//		{	 
//		 res ++;
////		CAN1_SendMsg(canbuf , 8, 0x200);
//		}
//		M3508[0].PID.Goal_Speed = res;			
//		POINT_COLOR=RED;//��������Ϊ��ɫ 
//		LCD_ShowString(60,130,200,16,16,"Nnormal Mode ");	    
//		key=Can_Receive_Msg(canbuf);
//		if(key)//���յ�������
//		{			
//			LCD_Fill(60,270,130,310,WHITE);//���֮ǰ����ʾ
// 			for(i=0;i<key;i++)
//			{									    
//				if(i<4)LCD_ShowxNum(60+i*32,270,canbuf[i],3,16,0X80);	//��ʾ����
//				else LCD_ShowxNum(60+(i-4)*32,290,canbuf[i],3,16,0X80);	//��ʾ����
// 			}
//		}
//   
//	}
//}



