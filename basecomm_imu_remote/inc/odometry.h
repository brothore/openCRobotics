#ifndef ODOMETRY_H
#define ODOMETRY_H


typedef struct
{
  float LeftSpeed;
	float RightSpeed;
	unsigned int LeftPusle;
	unsigned int RightPusle;
	float leftdistance;//��ʻ�ľ��� ��λm
	float Rightdistance;
}wheelInf;
extern wheelInf wheelParam;
extern float wheel_interval ;// 
extern float position_x;
extern float position_y;
extern float oriention;
extern float velocity_linear;
extern float velocity_angular;
extern float velocity_linear_x;
extern float velocity_linear_y;
extern float velocity_linear_l;
extern float velocity_linear_r;
extern void odometry(float right,float left);
extern void odometry_simple(float right,float left);
#endif