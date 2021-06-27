#include "odometry.h"
#include <math.h>

#include "stm32_control.h"

wheelInf wheelParam;
/***********************************************  ���  *****************************************************************/

float position_x=0;//��ʼλ��x������
float position_y=0;
float oriention=90*M_PI/180;//��ʼλ�õĽǶ�
float velocity_linear=0;//y���ϵ����ٶ�
float velocity_angular=0;//���ٶ�
float velocity_linear_x=0,velocity_linear_y=0;//x��y���ϵ����ٶ�
float velocity_linear_l=0,velocity_linear_r=0;
/***********************************************  ����  *****************************************************************/
 float odometry_right,odometry_left;//���ڵõ����������ٶ�

/***********************************************  ����  *****************************************************************/

float wheel_interval= 0.268f;//    272.0f;      m  //  1.0146
//float wheel_interval=276.089f;    //���У��ֵ=ԭ���/0.987

float multiplier=2.0f;           //��Ƶ��
float line_number=2.0f;       //��������  AB 2

float deceleration_ratio=35.0f;  //���ٱ� 170rpm 35
float wheel_diameter= 0.100f;     //����ֱ������λm
float pi_1_2=1.570796f;          //��/2
float pi=3.141593f;              //��
float pi_3_2=4.712389f;          //��*3/2
float pi_2_1=6.283186f;          //��*2
float dt=0.50f;                 //����ʱ����500ms

float pusle_cnt =11.0f;
float oriention_interval=0;  //dtʱ���ڷ���仯ֵ

float sin_=0;        //�Ƕȼ���ֵ
float cos_=0;

float delta_distance=0,delta_oriention=0;   //����ʱ�������˶��ľ���

float const_frame=0,const_angle=0,distance_sum=0,distance_diff=0;

float oriention_1=0;

char once=1;

/****************************************************************************************************************/

//��̼Ƽ��㺯�� ���ǿɺ���ƽ�� ����ƽ�Ƶļ���ŷ�
void odometry(float right,float left)
{   
	odometry_right = right;
	odometry_left = left;
    if(once)  //����������һ��
    {
			//���������Ĳ����й�ϵ
	//һ�����������ߵľ����Ƕ��٣�
	//���AB ����2�ߣ��������½��ض����������2��Ƶ һȦ11������ һȦ�ļ���Ϊ
	//11 *2 *2 �����������ٱ�Ϊ35 Ҳ��������������� 1/35Ȧ��ʱ 44��������
	// ��d һȦ���룬һ�������ľ����Ǧ�d*1/35 /44 
        const_frame=wheel_diameter*pi/(line_number*multiplier*deceleration_ratio*pusle_cnt);
        const_angle=const_frame/wheel_interval;
        once=0;
    }

    distance_sum = 0.5f*(odometry_right+odometry_left);//�ں̵ܶ�ʱ���ڣ�С����ʻ��·��Ϊ�����ٶȺ�
    distance_diff = right-left;//�ں̵ܶ�ʱ���ڣ�С����ʻ�ĽǶ�Ϊ�����ٶȲ�

    //���������ֵķ��򣬾�����ʱ���ڣ�С����ʻ��·�̺ͽǶ���������
    if((odometry_right>0)&&(odometry_left>0))            //���Ҿ���
    {
        delta_distance = distance_sum;
        delta_oriention = distance_diff;
    }
    else if((odometry_right<0)&&(odometry_left<0))       //���Ҿ���
    {
        delta_distance = -distance_sum;
        delta_oriention = -distance_diff;
    }
    else if((odometry_right<0)&&(odometry_left>0))       //�����Ҹ�
    {
        delta_distance = -distance_diff;
        delta_oriention = -2.0f*distance_sum;       
    }
    else if((odometry_right>0)&&(odometry_left<0))       //������
    {
        delta_distance = distance_diff;
        delta_oriention = 2.0f*distance_sum;
    }
    else
    {
        delta_distance=0;
        delta_oriention=0;
    }

    oriention_interval = delta_oriention * const_angle;//����ʱ�����ߵĽǶ�  
    oriention = oriention + oriention_interval;//�������̼Ʒ����
    oriention_1 = oriention + 0.5f * oriention_interval;//��̼Ʒ��������λ���仯���������Ǻ�������

    sin_ = sin(oriention_1);//���������ʱ����y����
    cos_ = cos(oriention_1);//���������ʱ����x����

    position_x = position_x + delta_distance * cos_ * const_frame;//�������̼�x����
    position_y = position_y + delta_distance * sin_ * const_frame;//�������̼�y����

	velocity_linear_x = delta_distance * cos_ * const_frame / dt;
	velocity_linear_y = delta_distance * sin_ * const_frame / dt;
    velocity_linear = 0.5f*(odometry_right+odometry_left)*const_frame / dt;//�������̼����ٶ�
    velocity_angular = distance_diff *const_angle / dt;//�������̼ƽ��ٶ�
	if(velocity_angular !=0 )
	{
	   int tmp = left;
	}
	velocity_linear_l = left;
	velocity_linear_r = right;
    //����ǽǶȾ���
    if(oriention > pi)
    {
        oriention -= pi_2_1;
    }
    else
    {
        if(oriention < -pi)
        {
            oriention += pi_2_1;
        }
    }
	printf("*************odom output********************\n");
	printf("linear vel:%3f,ang  vel:%3f,\n",velocity_linear,velocity_angular);
	printf("position_x:%3f,position_x:%3f,\n",position_x,position_y);
}

void odometry_simple(float right,float left)
{   
	  odometry_right = right;
	  odometry_left = left;
    if(once)  //����������һ��
    {
			//���������Ĳ����й�ϵ
	//һ�����������ߵľ����Ƕ��٣�
	//���AB ����2�ߣ��������½��ض����������2��Ƶ һȦ11������ һȦ�ļ���Ϊ
	//11 *2 *2 �����������ٱ�Ϊ35 Ҳ��������������� 1/35Ȧ��ʱ 44��������
	// ��d һȦ���룬һ�������ľ����Ǧ�d*1/35 /44 
        const_frame=wheel_diameter*pi/(line_number*multiplier*deceleration_ratio*pusle_cnt);
        const_angle=const_frame/wheel_interval;
        once=0;
    }
    
	//�������̼����ٶ� �������vy  ���ܺ���������vx =0;
    velocity_linear = 0.5f*(odometry_right+odometry_left)*const_frame / dt;
	//�������̼ƽ��ٶ�
    velocity_angular = (odometry_right -odometry_left) *const_angle / dt;
	velspeed = velocity_linear;
	angspeed = velocity_angular;
	double delta_x = velocity_linear * cos(oriention)  * dt;
	double delta_y = velocity_linear * sin(oriention)  * dt;
	double delta_th = velocity_angular * dt;
	//�������̼�x����
	position_x = position_x + delta_x;
	//�������̼�y����
    position_y = position_y + delta_y;
	 
	oriention += delta_th;
	if(oriention > pi)
    {
        oriention -= pi_2_1;
    }
    else if(oriention < -pi)
	{
		oriention += pi_2_1;
	}
	printf("odom:\n");
	printf("    vel:%3f\n",velocity_linear);
	printf("    ang:%3f,\n",velocity_angular);
	printf("    pose:\n");
	printf("        position_x:%3f,\n",position_x);
	printf("        position_y:%3f,\n",position_y);
    printf("        heading:%3f\n",oriention*180/3.1415);

    //����ǽǶȾ���

}

//https://blog.csdn.net/forrest_z/article/details/55001231 
