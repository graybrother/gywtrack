//
//  Header.h
//  gyw_test
//
//  Created by 郭义卫 on 14-4-30.
//  Copyright (c) 2014年 郭义卫. All rights reserved.
//

#include <string>
//最大定位数量
#define MAX_POSITION_NUM 10
//最大多边形交点数量
#define MAX_POLYGON_I_NUM 50
#define PI 3.1415926535898
//最大开方数
#define TABSQRTNUM 8000000
#define RECT 1
#define CIRCLE 2
#define ELLIPSE 3
#define POLYGON 4

#define POSITION 1
#define DETECT 2
#define BORDER 3

//op=0/不进行操作;op=1/创建ROI; op=2/清除ROI; op=3/清除全部ROI
#define OP_NULL 0
#define OP_CREAT 1
#define OP_CLEAR_ID 2
#define OP_CLEAR_ALLID 3

#define GRAY 1
#define SHAPE 2
#define COMPONENT 3
using  std::string;

typedef struct _coord
{
    int x;
    int y;
} coord_t;

typedef struct _coord_double
{
    double x;
    double y;
} coord_double_t;


typedef struct _img_Buffer
{
    int width;
    int height;
    unsigned char* ptr;
    
} img_Buffer_t;

//各种形状结构体-------------------------------shape
typedef struct _area_t
{
    coord_t       start;
    coord_t       end;
    coord_t       center;
}area_t;

typedef struct _line
{
    int ID;
    coord_t start;
    coord_t end;
} line_t;

typedef struct _rectangle_t
{
    coord_t start;
    coord_t end;

}rectangle_t;

typedef struct _circle_t
{
    coord_t  c;
    int     r;

}circle_t;

typedef struct _ellipse_t
{
    int         a;
    int         b;
    int         theta;
    coord_t     c;

}ellipse_t;

typedef struct _polygon_t
{

    int intersection_num;
    coord_t intersection[MAX_POLYGON_I_NUM];

}polygon_t;

//ROI数据结构--------------------------ROI
typedef struct _ROI_data_t
{
    int           ID;
    int           type;
    int           shape;
    area_t        area;
    rectangle_t   rect;
    circle_t      circle;
    ellipse_t     ellipse;
    polygon_t     polygon;

}ROI_data_t;

typedef struct _ROI_img_t
{
    int num;
    ROI_data_t *ptr;

}ROI_img_t;

//检测区域边界----------------------------border
typedef struct _border
{
    coord_t start;
    coord_t end;
} border_t;

typedef struct _position
{
    double matching;
    double angel;
    coord_double_t focus;
} position_t;

typedef struct _inROI_t
{
    int op;//op=0/不进行操作;op=1/创建ROI; op=2/清除ROI; op=255/清除全部ROI
    int ID;
    int shape;//形状
    int type;//检测类型
    int *dat;//第一个数据为数据尺寸

}inROI_t;

//输入参数
typedef struct _input_param
{
    //通道
    unsigned int ch;
    unsigned int imageFormat;
    //检测区域边界
    border_t border;
    inROI_t  inROI;

}input_param_t;
//输出参数
typedef struct _output
{
    int  sum;     //输出目标数量
    int  run_status; //dll状态
    int  x[100];  //输出目标坐标 x
    int  y[100];  //输出目标坐标 y
    int  w[100];  //输出目标宽度
    int  h[100];  //输出目标高度
    int  v[100];  //输出目标置信度（0~100）
    int  d[100];  //输出目标朝向（0=正面  1=左侧面 2=背面 3=右侧面）

}output_t;











