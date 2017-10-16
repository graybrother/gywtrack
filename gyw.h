//
//  gyw.h
//  Gyw_test
//
//  Created by 郭义卫 on 16/5/15.
//  Copyright © 2016年 郭义卫. All rights reserved.
//

#ifndef gyw_h
#define gyw_h
#include <header.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string.h>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv/cvaux.hpp>
//#include <opencv/cv.h>
#include <fstream>
#include <ctime>
#include <sys/time.h>
#include <queue>

#include "opencv.hpp"
using namespace cv;
using namespace std; //这几个头文件必不可少
//最大图像尺寸
#define MAX_IMG_W 4000
#define MAX_IMG_H 3000
//定义搜索所有步长的累积
#define MAX_A_STEPS 36000
//最大模板数量
#define MAX_ROIS_NUM 200
#define MAX_MODELS_NUM 2073600
#define LAYER_SUM 8
#define SEARCHSTEP 2

#pragma comment(lib, "cv.lib")
#pragma comment(lib, "cxcore.lib")
#pragma comment(lib, "highgui.lib")



typedef struct _yuv_color
{
    unsigned char y;
    unsigned char u;
    unsigned char v;

} yuv_color_t;

typedef struct _yuv422
{
    unsigned char u0;
    unsigned char y0;
    unsigned char v0;
    unsigned char y1;
    
} yuv422_t;

typedef struct _extreme_t
{
    int16_t max_x;
    int16_t min_x;
    int16_t max_y;
    int16_t min_y;
}extreme_t;

typedef struct _trans_param
{
    double angle;
    double scale;

}trans_param_t;

typedef struct _img_base_data
{
    unsigned char y;
    unsigned char u;
    unsigned char v;
} img_base_data_t;

//金字塔结构---------------------------------pyramid_img
typedef struct _pyramid_img
{
    int width;
    int height;
    img_base_data_t* ptr;

} pyramid_img_t;

//数据结构-------------------------------base_data
typedef struct _base_data
{
    short gradients;
    short orients;
    unsigned char gray;
    unsigned char edge_flag;
    coord_t pix;
    short Vt[2];//分析向量
    int blockID;
    bool MatchingDot;

} base_data_t;

typedef struct _base_img
{
    int width;
    int height;
    base_data_t *ptr;

} base_img_t;

typedef struct _target_unit
{
    int c_x;
    int c_y;
    int w;
    int h;
    int t;
    int d;
    int v;
    int intersect_num;
    int difference;
    int activity;
    int times;
    int flag;
    int GatherID;

}target_unit_t;

typedef struct _targets
{
    int  num;
    target_unit_t unit[1000];

}targets_t;

//边缘数据结构-------------------------------line
typedef struct _block
{
    int size;
    coord_t *ptr;
}block_t;

//模型数据结构---------------------------model
typedef struct _edge_model_dat
{
    coord_t       relPix;
    short         Vt[2];
    int           gradients;

} edge_model_dat_t;

typedef struct _edge_model_t
{
    int  size;
    edge_model_dat_t *ptr;
} edge_model_t;

//模型数据结构---------------------------model
typedef struct _gray_model_dat
{
    coord_t relPix;
    unsigned char  gray;

} gray_model_dat_t;

typedef struct _gray_model_t
{
    int  size;
    gray_model_dat_t *ptr;
} gray_model_t;

typedef struct _variance_t
{
    int            v;
    int            diff;
    int            ss;
} variance_t;

typedef struct _Global
{
    img_Buffer_t        *dis_img;

    int                 TOP_LAYER;
    pyramid_img_t       *pyramid_img[LAYER_SUM];
    ROI_img_t           *ROIs[LAYER_SUM];
    ROI_data_t           border[LAYER_SUM];
    base_img_t          *base_img[LAYER_SUM];//图像金字塔产生
    base_img_t           models_img[LAYER_SUM][MAX_MODELS_NUM];
    edge_model_dat_t     edge_models[LAYER_SUM][MAX_MODELS_NUM];
    gray_model_t         gray_models[LAYER_SUM][MAX_MODELS_NUM];
    int                 edge_matching_dat_num;
    int                 gray_matching_dat_num;
    int                 target_num;
    target_unit_t        targets[100];
    int loadFormat;
    int imageFormat;
   
    int run_mode;
    int mem_flag;
    
} Global_t;

inline short t_max(short a,short b);
inline short t_min(short a,short b);
inline unsigned char clip(short x);

void vertices_get_extent (coord_t *TopPoint, int TopPoint_num, extreme_t *rc);
int gen_polygon (polygon_t *polygon, coord_t *v);
int is_same_l( coord_t *start,coord_t *end,coord_t *p,coord_t *q);
int is_intersect( coord_t *s1_start, coord_t *s1_end, coord_t *s2_start,coord_t *s2_end);

int init_param(input_param_t input,img_Buffer_t *yuv);
int mix_frame_pro(input_param_t input,img_Buffer_t *yuv,output_t *output);

int free_memory(img_Buffer_t *yuv);
int malloc_memory(img_Buffer_t *yuv);

int rgb24_to_yuv422_packed_uyvy(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer);
int rgb32_to_yuv422_packed_uyvy(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer);
int rgb32_to_yuv422_packed_yuyv(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer);
int rgb32_to_yuv420p(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer);
int rgb32_to_yuv420sp(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer);
int yuv422_packed_uyvy_to_rgb24(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer);
int yuv422_packed_uyvy_to_rgb32(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer);
int yuv422_packed_yuyv_to_rgb32(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer);
int yuv420p_to_rgb24(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer);
int yuv420p_to_rgb32(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer);
int yuv420sp_to_rgb24(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer);
int yuv420sp_to_rgb32(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer);

void yuv_draw_orthogon(coord_t c, int w, int h,yuv_color_t color,img_Buffer_t *in_buffer);
void yuv_draw_rectangle(coord_t start,coord_t end,yuv_color_t color,img_Buffer_t *in_buffer);
void yuv_draw_star(coord_t c, int w, int h,yuv_color_t color,img_Buffer_t *in_buffer);
void LineWithDDA(coord_t start,coord_t end,yuv_color_t color,img_Buffer_t *in_buffer);
void yuv_draw_aiming(coord_t c, int w, int h,yuv_color_t color,img_Buffer_t *in_buffer);


void All_Right_Reserved(img_Buffer_t *img);
void display_imageFormat(input_param_t input,coord_t offset,yuv_color_t color,img_Buffer_t *img);

//字符显示部分
void set_pixel(coord_t offset,yuv_color_t color,img_Buffer_t *img);
void black_white_image(coord_t offset,yuv_color_t color,img_Buffer_t *img);
void disp_english(coord_t offset,char s_string,yuv_color_t color,img_Buffer_t *img);
void disp_english_string(coord_t offset,char s_string[],yuv_color_t color,img_Buffer_t *img);
void disp_variable(coord_t offset, int variable,yuv_color_t color,img_Buffer_t *img);
int creat_cutting_offset(output_t *target,img_Buffer_t *cutting,coord_t error);
int image_cutting(int format,output_t *target,img_Buffer_t *yuv_in,img_Buffer_t *cutting);
void display_selecting(input_param_t input,img_Buffer_t *img);

//开发商口令gyw-119360 用户口令11936000


#endif /* gyw_h */
