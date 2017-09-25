#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H
extern float field_width_;
extern float field_height_ ;
extern float line_width_ ;
extern float field_overlap_ ;
extern float unit ;
extern int units_y_;
extern int units_x_;
extern float thickness_;
extern float offset_dis_;
extern int * num_hatch;
//////////////////////////////////////////////////////////////////////////
extern float DEFAULT_L ;
extern float THRESHOLD;
extern float THRESHOLD1;
extern float GAP;
extern float SEGLENGTH ;
extern float RESO ;
extern float VERTICALGAP;
//////////////////////////////////////////////////////////////////////////
//extern float laser_power_ ;
//extern float laser_speed_;
extern float laser_power_hatch_ ;
extern float laser_speed_hatch_ ;
extern float laser_power_polygon_ ;
extern float laser_speed_polygon_ ;
extern int increment_angle_;
extern float scaleV;
extern float scaleT;
extern int sss;
extern int fildID;

extern int num;		//零碎的一些记录
extern int insStartPoint;	//相交边的start端点
extern int insEndPoint;	//相交边的End端点
extern int insRedunPoint;//相交边的冗余点

extern float INSPOINT_X;				//做全局变量来用，存储边相交的交点X坐标
extern float INSPOINT_Y;				//做全局变量来用，存储边相交的交点Y坐标
extern float INSPOINT_Z;				//做全局变量来用，存储边相交的交点Z坐标

extern bool updateFlag;

template<class object>
void SafeDelete(object *ptr)
{
	if (ptr != NULL)
	{
		delete ptr;
		ptr = NULL;
	}
}

template<class object>
void SafeDeletes(object *ptr)
{
	if (ptr != NULL)
	{
		delete[] ptr;
		ptr = NULL;
	}
}

template <typename T>
void inline MySwap(T &t1, T &t2)
{
	T tmp = t1;
	t1 = t2;
	t2 = tmp;
};

#endif // GLOBALFUNCTIONS_H
