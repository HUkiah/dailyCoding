#include "qmath.h"
float field_width_ = 5.0;
float field_height_ = 5.0;
float line_width_ = 0.150;
float field_overlap_ = 0.09;
float unit = 0.01;
int units_y_ = field_height_ / line_width_;
int units_x_ = field_width_ / line_width_;
float thickness_ = 0.03;
float offset_dis_ = 0.051;
int * num_hatch;
float laser_power_hatch_ = 200;
float laser_speed_hatch_ = 500;
float laser_power_polygon_ = 100;
float laser_speed_polygon_ = 700;
int increment_angle_ = 67;
float DEFAULT_L = 0.5f;
float THRESHOLD = cos(3.1415926 * 60 / 180);
float THRESHOLD1 = cos(3.1415926 * 70 / 180);
float GAP = 0.8f;
float SEGLENGTH = 1.5f;
float RESO = 0.5f;
float VERTICALGAP = 2.f;
float scaleV = 1.0;
float scaleT = 1.0;
int sss = 0;
int fildID = 0;

bool updateFlag = false;
int num = 0;		//充当计数的变量，还充当给三角面ID 赋值 在读取它们(面片)的时候
int insStartPoint=0;	//相交边的start端点
int insEndPoint=0;		//相交边的End端点
int insRedunPoint = 0;	//相交边的冗余点

float INSPOINT_X;				//做全局变量来用，存储边相交的交点X坐标
float INSPOINT_Y;				//做全局变量来用，存储边相交的交点Y坐标
float INSPOINT_Z;				//做全局变量来用，存储边相交的交点Z坐标