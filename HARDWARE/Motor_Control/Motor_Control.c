/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/

#include "Motor_Control.h"

struct M3508_T M3508[6];
extern int32_t frist_angle;
/*
	����: ����ṹ���ʼ������
	����: ��
	����ֵ: ��
*/
void Control_Initialize(void){
	// ���1
	{
		// �����е��
		M3508[0].Angle = 0;
		// ���Ť��
		M3508[0].Moment = 0;
		// �����ǰ�ٶ�
		M3508[0].Speed = 0;
		// ������������
		M3508[0].Control_Current = 0;
		M3508[0].Stop_On = 0;
		// ���ֹͣ��־
		M3508[0].Stop_Flag = 0;
		// ��������޷�ֵ(����ֵ)
		M3508[0].Current_Limit = 6000;
		M3508[0].Round = 0;
		M3508[0].Motor_Length = 0;
		M3508[0].Last_Angle = 0;
		// PID�ṹ���ʼ��
		M3508[0].PID.Kp = 2;
		M3508[0].PID.Ki = 0.5;
		M3508[0].PID.Kd = 0;
		M3508[0].PID.Last_Error = 0;
		M3508[0].PID.Goal_Speed = 0;
		M3508[0].PID.Error_Sum = 0;
	}
	// ���2
	{
		M3508[1].Angle = 0;
		M3508[1].Moment = 0;
		M3508[1].Speed = 0;
		M3508[1].Control_Current = 0;
		M3508[1].Stop_On = 0;
		M3508[1].Stop_Flag = 0;
		M3508[1].Current_Limit = 6000;
		M3508[1].Round = 0;
		M3508[1].Motor_Length = 0;
		M3508[1].Last_Angle = 0;
		// PID�ṹ���ʼ��
		M3508[1].PID.Kp = 2;
		M3508[1].PID.Ki = 0.5;
		M3508[1].PID.Kd = 0;
		M3508[1].PID.Last_Error = 0;
		M3508[1].PID.Goal_Speed = 0;
		M3508[1].PID.Error_Sum = 0;
	}
	// ���3
	{
		// �����е��
		M3508[2].Angle = 0;
		// ���Ť��
		M3508[2].Moment = 0;
		// �����ǰ�ٶ�
		M3508[2].Speed = 0;
		// ������������
		M3508[2].Control_Current = 0;
		M3508[2].Stop_On = 0;
		// ���ֹͣ��־
		M3508[2].Stop_Flag = 0;
		// ��������޷�ֵ(����ֵ)
		M3508[2].Current_Limit = 6000;
		M3508[2].Round = 0;
		M3508[2].Motor_Length = 0;
		M3508[2].Last_Angle = 0;
		// PID�ṹ���ʼ��
		M3508[2].PID.Kp = 2;
		M3508[2].PID.Ki = 0.5;
		M3508[2].PID.Kd = 0;
		M3508[2].PID.Last_Error = 0;
		M3508[2].PID.Goal_Speed = 0;
		M3508[2].PID.Error_Sum = 0;
	}
	// ���4
	{
		M3508[3].Angle = 0;
		M3508[3].Moment = 0;
		M3508[3].Speed = 0;
		M3508[3].Control_Current = 0;
		M3508[3].Stop_On = 0;
		M3508[3].Stop_Flag = 0;
		M3508[3].Current_Limit = 6000;
		M3508[3].Round = 0;
		M3508[3].Motor_Length = 0;
		M3508[3].Last_Angle = 0;
		// PID�ṹ���ʼ��
		M3508[3].PID.Kp = 2;
		M3508[3].PID.Ki = 0.5;
		M3508[3].PID.Kd = 0;
		M3508[3].PID.Last_Error = 0;
		M3508[3].PID.Goal_Speed = 0;
		M3508[3].PID.Error_Sum = 0;
	}
	// ���5
	{
		M3508[4].Angle = 0;
		M3508[4].Moment = 0;
		M3508[4].Speed = 0;
		M3508[4].Control_Current = 0;
		M3508[4].Stop_On = 0;
		M3508[4].Stop_Flag = 0;
		M3508[4].Current_Limit = 6000;
		M3508[4].Round = 0;
		M3508[4].Motor_Length = 0;
		M3508[4].Last_Angle = 0;
		// PID�ṹ���ʼ��
		M3508[4].PID.Kp = 2;
		M3508[4].PID.Ki = 0.5;
		M3508[4].PID.Kd = 0;
		M3508[4].PID.Last_Error = 0;
		M3508[4].PID.Goal_Speed = 0;
		M3508[4].PID.Error_Sum = 0;
	}
	// ���6
	{
		M3508[5].Angle = 0;
		M3508[5].Moment = 0;
		M3508[5].Speed = 0;
		M3508[5].Control_Current = 0;
		M3508[5].Stop_On = 0;
		M3508[5].Stop_Flag = 0;
		M3508[5].Current_Limit = 6000;
		M3508[5].Round = 0;
		M3508[5].Motor_Length = 0;
		M3508[5].Last_Angle = 0;
		// PID�ṹ���ʼ��
		M3508[5].PID.Kp = 2;
		M3508[5].PID.Ki = 0.5;
		M3508[5].PID.Kd = 0;
		M3508[5].PID.Last_Error = 0;
		M3508[5].PID.Goal_Speed = 0;
		M3508[5].PID.Error_Sum = 0;
	}
}


/*
	����: ����ת������(��ԭʼ���ݵõ�������)
	����: ������(1, 2, 3, 4,5,6)
	����ֵ: ��
*/
void Data_Convert(u8 Motor_ID){
	M3508[Motor_ID].Angle = (int16_t)((M3508[Motor_ID].Original_Data[0] << 8) + M3508[Motor_ID].Original_Data[1]);
	M3508[Motor_ID].Speed = (int16_t)((M3508[Motor_ID].Original_Data[2] << 8) + M3508[Motor_ID].Original_Data[3]);
	M3508[Motor_ID].Moment = (int16_t)((M3508[Motor_ID].Original_Data[4] << 8) + M3508[Motor_ID].Original_Data[5]);
}

/*
	����: �����������͸����(�Ѵ�����)
	����: ��
	����ֵ: ��
*/
void CAN_Send_Control_Value(void){
	u8 i;
	static u8 Motor14_Current[8],Motor58_Current[8];
	static u8 First_Flag = 1;
	if(First_Flag){
		for(i = 0; i < 8; ++i)
		{
			Motor14_Current[i] = 0;
			Motor58_Current[i] = 0;
		}
		First_Flag = 0;
	}
	// ����CAN��
	Motor14_Current[0] = (u8)(((M3508[0].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[1] = (u8)((M3508[0].Control_Current) & 0x00FF);
	
//	Motor14_Current[2] = (u8)(((M3508[1].Control_Current) & 0xFF00) >> 8);
//	Motor14_Current[3] = (u8)((M3508[1].Control_Current) & 0x00FF);
//	
//	Motor14_Current[4] = (u8)(((M3508[2].Control_Current) & 0xFF00) >> 8);
//	Motor14_Current[5] = (u8)((M3508[2].Control_Current) & 0x00FF);
//	
//	Motor14_Current[6] = (u8)(((M3508[3].Control_Current) & 0xFF00) >> 8);
//	Motor14_Current[7] = (u8)((M3508[3].Control_Current) & 0x00FF);
//	
//	Motor58_Current[0] = (u8)(((M3508[4].Control_Current) & 0xFF00) >> 8);
//	Motor58_Current[1] = (u8)((M3508[4].Control_Current) & 0x00FF);
//	
//	Motor58_Current[2] = (u8)(((M3508[5].Control_Current) & 0xFF00) >> 8);
//	Motor58_Current[3] = (u8)((M3508[5].Control_Current) & 0x00FF);
//	
//	Motor58_Current[4] = 0;
//	Motor58_Current[5] = 0;
//	
//	Motor58_Current[6] = 0;
//	Motor58_Current[7] = 0;
	
	// ��� 1 ~ 4 0x200
	// ��� 5 ~ 8 0x1FF
	CAN1_SendMsg(Motor14_Current, 8, 0x200);
//	CAN1_SendMsg(Motor58_Current, 8, 0x1FF);
}

/*
	����: ���·�̼���
	����: ��
	����ֵ: ��
	��ע: �˺�����Ҫ�Ż�(�����ٶȽ����Ż�)
*/
void Motor_Length_Calc(void){
	u8 i;
	for(i = 0; i < 2; ++i){
		// ���ͻ��
		if((M3508[i].Angle - M3508[i].Last_Angle) < -4000){
			
			M3508[i].Last_Angle = M3508[i].Angle;
			// �Ѿ�һȦ��
			M3508[i].Round += 360;
		}
		else if((M3508[i].Angle - M3508[i].Last_Angle) > 4000){
			M3508[i].Last_Angle = M3508[i].Angle;
			// �Ѿ�һȦ��
			M3508[i].Round -= 360;
		}
		M3508[i].Motor_Length = M3508[i].Round + M3508[i].Angle * 0.043951;
	}
}

void Motor_Length_Calc_2(void){
	// �ٶȻ���
	M3508[0].Motor_Length += M3508[0].Speed * 0.01;
}

/*
	����: ���λ�ÿ���
	����: ���Ŀ��λ��
         ����ٶ�(����ֵ)
	����ֵ: (1 Ϊ�Ѿ��ﵽ)(0 Ϊû�дﵽ)
*/
u8 Motor_Position_Control(float Goal_Position, int16_t Speed){
	u8 Return_Flag = 1;
	if(fabs(M3508[0].Motor_Length - Goal_Position) > 2000){
		if(M3508[0].Motor_Length < Goal_Position){
			// �ٶȵ���������Ҫ��
			Motor_Speed_Control(Speed, 0);
			Motor_Speed_Control(-Speed, 1);
		}
		else{
			// �ٶȵ���������Ҫ��
			Motor_Speed_Control(-Speed, 0);
			Motor_Speed_Control(Speed, 1);
		}
		// δ�ﵽĿ��λ��
		Return_Flag = 0;
	}
	else{
		Motor_Speed_Control(0, 0);
		Motor_Speed_Control(0, 1);
		// �ѴﵽĿ��λ��
		Return_Flag = 1;
	}
	CAN_Send_Control_Value();
	return Return_Flag;
}

/*
	����: �������
	����: ��
	����ֵ:(1 ��������) (0 ����������)
*/
u8 Motor_Protect(void){
	static u8 Stop_Count = 0;
	u8 Protect_Flag = 0;
	// �������(����)
	if((fabs(M3508[0].Speed) < 100) || (fabs(M3508[1].Speed) < 100)){
		if((fabs(M3508[0].Control_Current) > 2000) || (fabs(M3508[1].Control_Current) > 2000)){
			Stop_Count++;
		}
	}
	if(Stop_Count > 10){
		Protect_Flag = 1;
		M3508[0].Stop_Flag = 1;
		M3508[1].Stop_Flag = 1;
	}
	return Protect_Flag;
}


/***����Ƕ����������****/
int32_t RM820R_Ang2Cnt(int ang)
{
	int32_t cnt = (CNT_PER_ROUND_OUT * ang) / 360.0 + frist_angle;
	return(cnt);
}

/****��������������Ƕ�****/
double RM820R_Cnt2Ang(int32_t cnt){
	return (double)(((cnt - frist_angle) * 360.0)/CNT_PER_ROUND_OUT);
}
