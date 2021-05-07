#ifndef __PID_H
#define __PID_H	 

#include "Motor_Control.h"



typedef struct{
  float Kp;
  float Ki;
  float Kd;
		
	float  SetPoint; 	//  �趨Ŀ�� Desired Value 
	double  SumError;		//	����ۼ� 
		
	float  Proportion;      //  �������� Proportional Const 
	float  Integral;        //  ���ֳ��� Integral Const
	float  Derivative;      //  ΢�ֳ��� Derivative Const

	float LastError;     //  Error[-1]
	float PrevError;     //  Error[-2]
	
	double MaxLimit;
}PIDTypDef;


extern PIDTypDef  PPM_PID;

void PID_PPM_SetKp(float dKpp);
void PID_PPM_SetKi(float dKii);
void PID_PPM_SetKd(float dKdd);

void Motor_Speed_Control(int32_t Goal_Speed, u8 Motor_ID);
double Motor_point_PID(int32_t Goal_point,double Current_point);
void ppm_pidinit(void);

#endif




