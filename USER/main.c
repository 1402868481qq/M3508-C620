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


//任务优先级
#define START_TASK_PRIO			1
//任务堆栈大小	
#define START_STK_SIZE 			256  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define KEY_TASK_PRIO		2
//任务堆栈大小	
#define KEY_STK_SIZE 		128  
//任务句柄
TaskHandle_t KeyTask_Handler;
//任务函数
void key_task(void *pvParameters);

//任务优先级
#define TASK1_TASK_PRIO		3
//任务堆栈大小	
#define TASK1_STK_SIZE 		128  
//任务句柄
TaskHandle_t Task1Task_Handler;
//任务函数
void task1_task(void *pvParameters);

//任务优先级
#define PVM_TASK_PRIO		4
//任务堆栈大小	
#define PVM_STK_SIZE 		128  
//任务句柄
TaskHandle_t PVMTask_Handler;
//任务函数
void pvm_task(void *pvParameters);

#if PPM_control
//任务优先级
#define PPM_TASK_PRIO		5
//任务堆栈大小	
#define PPM_STK_SIZE 		256  
//任务句柄
TaskHandle_t PPMTask_Handler;
//任务函数
void ppm_task(void *pvParameters);
#endif

float kppp;
float kiii;
float kddd;

int32_t frist_angle = 0;
int point_angle = 0;
int32_t  angle_cnt = 0;
double point_pidout = 0;

int main(void)
{
	 delay_init();	    	 //延时函数初始化	  
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置中断优先级分组为组4     0~15位抢占优先级  没有子优先级
	 uart_init(115200);	 	//串口初始化为115200
	 LED_Init();		  		//初始化与LED连接的硬件接口
	 LCD_Init();			   	//初始化LCD	
	 KEY_Init();				  //按键初始化		 	
   Control_Initialize(); 
   CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,2,CAN_Mode_Normal);//CAN普通模式初始化, 波特率1MKbps 
	 TIM3_Int_Init(15, 36000-1);   // 8ms中断一次
	 ppm_pidinit();
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
    //创建中断测试任务
	//创建KEY任务
	xTaskCreate((TaskFunction_t )key_task,             
                (const char*    )"key_task",           
                (uint16_t       )KEY_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )KEY_TASK_PRIO,        
                (TaskHandle_t*  )&KeyTask_Handler);  
   //创建TASK1任务
  xTaskCreate((TaskFunction_t )task1_task,             
                (const char*    )"task1_task",           
                (uint16_t       )TASK1_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TASK1_TASK_PRIO,        
                (TaskHandle_t*  )&Task1Task_Handler);   							
	//创建速度环任务			
 	 xTaskCreate((TaskFunction_t )pvm_task,             
                (const char*    )"pvm_task",           
                (uint16_t       )PVM_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )PVM_TASK_PRIO,        
                (TaskHandle_t*  )&PVMTask_Handler); 
 	#if PPM_control
	//创建角度环任务	
	 xTaskCreate((TaskFunction_t )ppm_task,             
                (const char*    )"ppm_task",           
                (uint16_t       )PPM_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )PPM_TASK_PRIO,        
                (TaskHandle_t*  )&PPMTask_Handler); 
  #endif								
	vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}


//key任务函数
void key_task(void *pvParameters)
{
	u8 key = 0;
	while(1)
	{
		key=KEY_Scan(0);
		switch(key)
		{
			case WKUP_PRES:
       vTaskSuspend(Task1Task_Handler); //挂起任务
				break;
			case KEY1_PRES:
       vTaskResume(PPMTask_Handler);	 //恢复任务
			  break;
			case KEY0_PRES:
				vTaskSuspend(PPMTask_Handler); //挂起任务
			  LED0=1;     //灯灭
				break;
		}
		vTaskDelay(10);			//延时10ms 
	}
}


//task1任务函数
void task1_task(void *pvParameters)
{
	u8 task1_num=0;
	
//	POINT_COLOR = BLACK;

//	LCD_DrawRectangle(5,110,115,314); 	//画一个矩形	
//	LCD_DrawLine(5,130,115,130);		//画线
	POINT_COLOR = MAGENTA;
	LCD_ShowString(6,111,110,16,16,"Task1 Run:000");
	while(1)
	{
		task1_num++;	//任务执1行次数加1 注意task1_num1加到255的时候会清零！！
//		LED0=!LED0;
    printf("任务1已经执行：%d次\r\n",task1_num);
		LCD_ShowxNum(86,111,task1_num,3,16,0x80);	//显示任务执行次数
    vTaskDelay(1000);                           //延时1s，也就是1000个时钟节拍	
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
		frist_angle = M3508[0].Angle;                 //记住初始机械角度
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
					angle_cnt = RM820R_Ang2Cnt(point_angle);            //计算目标角度的脉冲数
					point_pidout =  Motor_point_PID(angle_cnt,M3508[0].CNT_ABS);    //角度环PID输出
					Motor_Speed_Control((int32_t)point_pidout, 0);                  //速度环，电机0，ID号 1
					CAN_Send_Control_Value();                                       //CAN发送电流控制数据
					ANO_DT_Send_Senser();                                           //匿名地面站
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
//	delay_init();	    	 //延时函数初始化	  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
//	uart_init(115200);	 	//串口初始化为115200
//	LED_Init();		  		//初始化与LED连接的硬件接口
//	LCD_Init();			   	//初始化LCD	
//	KEY_Init();				  //按键初始化		 	
//  Control_Initialize(); 
////	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,2,CAN_Mode_LoopBack);//CAN初始化环回模式,波特率500Kbps    
//  CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,2,CAN_Mode_Normal);//CAN普通模式初始化, 波特率1MKbps 
//	TIM3_Int_Init(15, 36000-1);   // 8ms中断一次
// 	POINT_COLOR=RED;//设置字体为红色 
//	LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
//	LCD_ShowString(60,70,200,16,16,"CAN TEST");	
//	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
//	LCD_ShowString(60,110,200,16,16,"2015/1/15");
//	LCD_ShowString(60,130,200,16,16,"LoopBack Mode");	 
//	LCD_ShowString(60,150,200,16,16,"KEY0:Send WK_UP:Mode");//显示提示信息		
//	POINT_COLOR=BLUE;//设置字体为蓝色	  
//	LCD_ShowString(60,170,200,16,16,"Count:");			//显示当前计数值	
//	LCD_ShowString(60,190,200,16,16,"Send Data:");		//提示发送的数据	
//	LCD_ShowString(60,250,200,16,16,"Receive Data:");	//提示接收到的数据		
// 	while(1)
//	{	   
//		key=KEY_Scan(0);
//		if(key==KEY0_PRES)//KEY0按下,发送一次数据
//		{	 
//		 res ++;
////		CAN1_SendMsg(canbuf , 8, 0x200);
//		}
//		M3508[0].PID.Goal_Speed = res;			
//		POINT_COLOR=RED;//设置字体为红色 
//		LCD_ShowString(60,130,200,16,16,"Nnormal Mode ");	    
//		key=Can_Receive_Msg(canbuf);
//		if(key)//接收到有数据
//		{			
//			LCD_Fill(60,270,130,310,WHITE);//清除之前的显示
// 			for(i=0;i<key;i++)
//			{									    
//				if(i<4)LCD_ShowxNum(60+i*32,270,canbuf[i],3,16,0X80);	//显示数据
//				else LCD_ShowxNum(60+(i-4)*32,290,canbuf[i],3,16,0X80);	//显示数据
// 			}
//		}
//   
//	}
//}



