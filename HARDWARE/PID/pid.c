#include "pid.h"

PIDTypDef  PPM_PID;


void ppm_pidinit(void)
{
    PPM_PID.Kp=0.0;
    PPM_PID.Ki=0.0;   
	  PPM_PID.Kd=0.0;
	
	  PPM_PID.LastError = 0.0;
	  PPM_PID.MaxLimit = 8000.0;   //����Ľ���Ϊ�ٶȻ�����   �ٶ�����8000  ��Ҫ��������  ��ͬʱ�޸��ٶȻ��ĵ������
}



//��ʱ����
void PVM3508_init(PIDTypDef *PID)
{
    PID->Kp = 2.0;
    PID->Ki = 0.5;
    PID->Kd = 0.0;

	  PID->SumError = 0.0;
	  PPM_PID.LastError = 0.0;
	  PPM_PID.MaxLimit  = 6000.0;     
}

void PPM3508_init(PIDTypDef *PID)
{
    PID->Kp = 0.2;
    PID->Ki = 0.0;
    PID->Kd = 0.38;
	 
   	PID->SumError = 0.0;	
	  PPM_PID.LastError = 0.0;
	  PPM_PID.MaxLimit  = 8000.0;     
}


/*------------------------------------------
 ��������:����PID����ϵ��
 ����˵��:������			
------------------------------------------*/
void PID_PPM_SetKp(float dKpp)
{
     PPM_PID.Kp=dKpp;
}

void PID_PPM_SetKi(float dKii)
{
     PPM_PID.Ki=dKii;
}

void PID_PPM_SetKd(float dKdd)
{
     PPM_PID.Kd=dKdd;
}


/*
	����: ����ٶȿ��ƺ���(PID)(�ڲ����޷�)(�����ǻ�ȡ��������������)
	����: Ŀ���ٶ�
         ������(5,6)
	����ֵ: ��
*/
void Motor_Speed_Control(int32_t Goal_Speed, u8 Motor_ID){
	double Error;
	double Control_Current = 0;
	// ����ƫ��
	Error = Goal_Speed - M3508[Motor_ID].Speed;
	M3508[Motor_ID].PID.Error_Sum += Error;
	// PID�����޷�
	if(fabs(M3508[Motor_ID].PID.Error_Sum) > 2000){
		if(M3508[Motor_ID].PID.Error_Sum > 0) M3508[Motor_ID].PID.Error_Sum = 2000;
		else M3508[Motor_ID].PID.Error_Sum = -2000;
	}
	// ������������
	Control_Current = M3508[Motor_ID].PID.Kp * Error +
										M3508[Motor_ID].PID.Ki * M3508[Motor_ID].PID.Error_Sum + 
										M3508[Motor_ID].PID.Kd * (Error - M3508[Motor_ID].PID.Last_Error);
	M3508[Motor_ID].PID.Last_Error = Error;
	// �����޷�
	if(fabs(Control_Current) < 1) Control_Current = 0;
	else{
		// �޷�
		if(fabs(Control_Current) > M3508[Motor_ID].Current_Limit){
			if(Control_Current > 0) Control_Current = M3508[Motor_ID].Current_Limit;
			else Control_Current = -M3508[Motor_ID].Current_Limit;
		}
		else{
			Control_Current = (int16_t)(Control_Current);
		}
	}
	M3508[Motor_ID].Control_Current = Control_Current;
}


double Motor_point_PID(int32_t Goal_point,double Current_point){
	float Point_Error;
	double Control_Current = 0;

	Point_Error = Goal_point - Current_point;
	PPM_PID.SumError  += Point_Error;
	if(fabs(PPM_PID.SumError) > 2000){
		if(PPM_PID.SumError > 0) PPM_PID.SumError = 2000;
		else PPM_PID.SumError = -2000;
	}
	
	// ���������
	Control_Current = PPM_PID.Kp * Point_Error +
										PPM_PID.Ki * PPM_PID.SumError+ 
										PPM_PID.Kd * (Point_Error - PPM_PID.LastError);
	PPM_PID.LastError = Point_Error;
	
	if(Control_Current > PPM_PID.MaxLimit) Control_Current = PPM_PID.MaxLimit;
	
	else if(Control_Current < -PPM_PID.MaxLimit) Control_Current = -PPM_PID.MaxLimit;
	
	return Control_Current;
}



