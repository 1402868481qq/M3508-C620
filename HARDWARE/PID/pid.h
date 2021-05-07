#ifndef __PID_H
#define __PID_H	 

#include "Motor_Control.h"



typedef struct{
  float Kp;
  float Ki;
  float Kd;
		
	float  SetPoint; 	//  设定目标 Desired Value 
	double  SumError;		//	误差累计 
		
	float  Proportion;      //  比例常数 Proportional Const 
	float  Integral;        //  积分常数 Integral Const
	float  Derivative;      //  微分常数 Derivative Const

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




