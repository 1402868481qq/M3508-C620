/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/

#include "tim.h"


/**************************************************************************
�������ܣ���ʱ��3��ʼ��
��ڲ�����arr:��װ��ֵ , psc: Ԥ��Ƶֵ
����  ֵ����
**************************************************************************/

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}


/**************************************************************************
�������ܣ���ʱ��3�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
int angle = 0;
int speed = 0;
u8 flag = 0;
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	static u8 j;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
			static u8 i=0;
      TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
	    j++;
			if( j == 60 )
			{
			LED1=!LED1;
		  j = 0;
			}
//      ���Ƶ���ٶ�		
//			Motor_Speed_Control(M3508[0].PID.Goal_Speed, 0);
//			Motor_Speed_Control(M3508[1].PID.Goal_Speed, 1);
//			Motor_Speed_Control(M3508[2].PID.Goal_Speed, 2);
//			Motor_Speed_Control(M3508[3].PID.Goal_Speed, 3);
//			Motor_Speed_Control(M3508[4].PID.Goal_Speed, 4);
//			Motor_Speed_Control(M3508[5].PID.Goal_Speed, 5);
//			CAN_Send_Control_Value();
//			Motor_Length_Calc();
//			Motor_Length_Calc_2();
//			flag = Motor_Position_Control(angle,speed);
			
			
			//��λ������λ	
//			get_pot_adc();
//			adc_angle_transform();
//			
//			if(angle[0]>233) M3508[2].PID.Goal_Speed = 0;
//			if(angle[1]>233) M3508[3].PID.Goal_Speed = 0;
//			if(angle[2]>233) M3508[4].PID.Goal_Speed = 0;
//			if(angle[3]>233) M3508[5].PID.Goal_Speed = 0;
			

			//printf("M1_Goal_Speed:%d\r\n",M3508[0].PID.Goal_Speed);
			//printf("M2_Goal_Speed:%d\r\n",M3508[1].PID.Goal_Speed);
			
			
//			printf("%d,%d,%d,%d\n",M3508[0].PID.Goal_Speed,M3508[0].Speed,M3508[1].PID.Goal_Speed,M3508[1].Speed);
//			if(i>9) 
//			{
//				printf("M1_Speed:%d\r\n",M3508[0].Speed);
//				printf("M1_Goal_Speed:%d\r\n",M3508[0].PID.Goal_Speed);
//				i=0;
//			}
//			else i++;
//			Motor_Info_Send();
			
//		send_can_data(angle[0],angle[1],(0x11e0>>5));
//		delay_ms(10);  //��ǰ��3   ���ڣ�10    �������ʱ��CANͨ�Ų��ȶ�
//		send_can_data(angle[2],angle[3],(0x12e0>>5));
		  // LED1=!LED1;
			//Motor_Info_Send();//
		}
}

