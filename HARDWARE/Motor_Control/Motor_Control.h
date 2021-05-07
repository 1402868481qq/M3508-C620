/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/

#ifndef __MOTOR_CONTROL__
#define __MOTOR_CONTROL__
#include "sys.h"
#include <math.h>
#include "can.h"
#include "pid.h"

#define   PPM_control    1
#define CNT_PER_ROUND 8192   //��ת��תһȦ��������
#define REDUCTION_RATIO 19   //���ٱ�
#define MOTOR_TOLERANCE 8192   //λ�û��ݲ�
#define CNT_PER_ROUND_OUT (CNT_PER_ROUND * REDUCTION_RATIO)     //�����תһȦ��������


struct PID_T{
	double Kp;
	double Ki;
	double Kd;
	double Last_Error;
	double Error_Sum;
	// ���Ŀ���ٶ�
	int32_t Goal_Speed;
};
struct M3508_T{
	// ��е�Ƕ�
	int32_t Angle;
	// ����ٶ�
	int32_t Speed;
	// �������
	int32_t Moment;
	// ԭʼ�ٶ�
	u8 Original_Data[8];
	// ������Ƶ���
	int16_t Control_Current;
	// ���ֹͣ����
	u8 Stop_On;
	// ���ֹͣ��־
	u8 Stop_Flag;
	// �����޷�
	int16_t Current_Limit;
	// ���·��:�ɽǶȳ���ϵ�����
	float Round;
	float Motor_Length;
	// �����һ�νǶ�
	int32_t Last_Angle;
	
	int32_t CNT_ABS;
	// PID
	struct PID_T PID;
};
extern struct M3508_T M3508[6];

extern void CAN_Send_Control_Value(void);//���Ϳ�����Ϣ
extern void Data_Convert(u8 Motor_ID); //����ת��
extern void Motor_Speed_Control(int32_t Goal_Speed, u8 Motor_ID); //PID����
extern void Control_Initialize(void); //����ṹ���ʼ��
extern void Motor_Length_Calc(void);  //���·�̼���
extern u8 Motor_Position_Control(float Goal_Position, int16_t Speed);  //���λ�ÿ���
extern void Motor_Length_Calc_2(void); //
extern u8 Motor_Protect(void);  //�������
int32_t RM820R_Ang2Cnt(int ang);
double RM820R_Cnt2Ang(int32_t cnt);
#endif


