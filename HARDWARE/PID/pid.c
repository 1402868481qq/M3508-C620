#include "pid.h"

PIDTypDef  PPM_PID;


void ppm_pidinit(void)
{
    PPM_PID.Kp=0.0;
    PPM_PID.Ki=0.0;   
	  PPM_PID.Kd=0.0;
	
	  PPM_PID.LastError = 0.0;
	  PPM_PID.MaxLimit = 8000.0;   //输出的将作为速度环输入   速度上线8000  如要继续上升  需同时修改速度环的电流输出
}



//暂时不用
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
 函数功能:设置PID比例系数
 函数说明:浮点型			
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
	描述: 电机速度控制函数(PID)(内部带限幅)(仅仅是获取控制量，不发送)
	参数: 目标速度
         电机电调(5,6)
	返回值: 无
*/
void Motor_Speed_Control(int32_t Goal_Speed, u8 Motor_ID){
	double Error;
	double Control_Current = 0;
	// 计算偏差
	Error = Goal_Speed - M3508[Motor_ID].Speed;
	M3508[Motor_ID].PID.Error_Sum += Error;
	// PID积分限幅
	if(fabs(M3508[Motor_ID].PID.Error_Sum) > 2000){
		if(M3508[Motor_ID].PID.Error_Sum > 0) M3508[Motor_ID].PID.Error_Sum = 2000;
		else M3508[Motor_ID].PID.Error_Sum = -2000;
	}
	// 计算电流输出量
	Control_Current = M3508[Motor_ID].PID.Kp * Error +
										M3508[Motor_ID].PID.Ki * M3508[Motor_ID].PID.Error_Sum + 
										M3508[Motor_ID].PID.Kd * (Error - M3508[Motor_ID].PID.Last_Error);
	M3508[Motor_ID].PID.Last_Error = Error;
	// 电流限幅
	if(fabs(Control_Current) < 1) Control_Current = 0;
	else{
		// 限幅
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
	
	// 计算输出量
	Control_Current = PPM_PID.Kp * Point_Error +
										PPM_PID.Ki * PPM_PID.SumError+ 
										PPM_PID.Kd * (Point_Error - PPM_PID.LastError);
	PPM_PID.LastError = Point_Error;
	
	if(Control_Current > PPM_PID.MaxLimit) Control_Current = PPM_PID.MaxLimit;
	
	else if(Control_Current < -PPM_PID.MaxLimit) Control_Current = -PPM_PID.MaxLimit;
	
	return Control_Current;
}



