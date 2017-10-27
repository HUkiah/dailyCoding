#include "qmath.h"
#include "globalFunctions.h"
#include "HE_mesh/Mesh3D.h"
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


int num = 0;		//�䵱�����ı��������䵱��������ID ��ֵ �ڶ�ȡ����(��Ƭ)��ʱ��
int insStartPoint=0;	//�ཻ�ߵ�start�˵�
int insEndPoint=0;		//�ཻ�ߵ�End�˵�
int insRedunPoint = 0;	//�ཻ�ߵ������

float INSPOINT_X;				//��ȫ�ֱ������ã��洢���ཻ�Ľ���X����
float INSPOINT_Y;				//��ȫ�ֱ������ã��洢���ཻ�Ľ���Y����
float INSPOINT_Z;				//��ȫ�ֱ������ã��洢���ཻ�Ľ���Z����

bool actionMaintenanceFlag = true;

int intersect_num=0;
int overlap_num = 0;
int reverse_num = 0;
int badside_num = 0;
int hole_num = 0;
int shell_num = 0;

bool holeMark=false;
bool badsideMark = false;